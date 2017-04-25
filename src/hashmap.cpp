#include "hashmap.hpp"
#include "csv.hpp"
#include <sparsepp/spp.h>

using spp::sparse_hash_map;

Hashmap::Hashmap() {};

Hashmap::Hashmap(const std::string &input_filename) {

    io::CSVReader<3> in(input_filename);
    in.set_header("from", "to", "speed");
    std::uint64_t from; std::uint64_t to; int speed;
    while(in.read_row(from, to, speed)){
        add(to, from, speed);
    }

};

// Hashmap::Hashmap(const std::string input_filename) {
//     std::ifstream input(input_filename, std::ifstream::in);

//     std::string line;
//     external_nodeid_t to;
//     external_nodeid_t from;
//     congestion_speed_t speed;
//     std::string str_to;
//     std::string str_from;
//     std::string str_speed;

//     if (input)
//     {
//         while (getline(input, line))
//         {
//             try
//             {
//                 std::stringstream iss;
//                 iss << line;
//                 std::getline(iss, str_to, ',');
//                 to = std::stoull(str_to);
//                 std::getline(iss, str_from, ',');
//                 from = std::stoull(str_from);
//                 std::getline(iss, str_speed, ',');
//                 speed = std::stoull(str_speed);

//                 add(to, from, speed);
//             }
//             catch (std::exception& e)
//             {
//                 std::cout << "Input input has invalid format." << std::endl;
//                 std::cout << e.what() << std::endl;
//             }
//         }
//     }
//     else {
//         BOOST_ASSERT_MSG(false, 'no inputfile');
//     }
//     input.close();
// };

void Hashmap::add(external_nodeid_t to, external_nodeid_t from, congestion_speed_t speed) {
    Hashmap::annotations[Way(to, from)] = speed;
};

bool Hashmap::hasKey(external_nodeid_t to, external_nodeid_t from) const {
    const auto lookup = annotations.find(Way(to,from));
    if(lookup != annotations.end()) {
        return true;
    } else {
        return false;
    }
};

// @TODO use hasKey to get pointer and directly return
congestion_speed_t Hashmap::getValue(external_nodeid_t to, external_nodeid_t from) const
{
    if (!Hashmap::hasKey(to, from)) {
        throw std::runtime_error("Way from NodeID " + std::to_string(to) + "to NodeId " + std::to_string(from) + " doesn't exist in the hashmap.");
    }

    return annotations.at(Way(to, from));
};

std::vector<congestion_speed_t> Hashmap::getValues(std::vector<external_nodeid_t>& way) const
{
    std::vector<congestion_speed_t> speeds;
    if (way.size() > 1)
    {
        speeds.resize(way.size() - 1);
        for (std::size_t node_id = 0; node_id < speeds.size(); node_id++)
        {
            if (hasKey(way[node_id], way[node_id+1]))
            {
                speeds[node_id] = getValue(way[node_id], way[node_id+1]);
            }
            else
            {
                speeds[node_id] = INVALID_SPEED;
            }
        }

    }
    return speeds;
};
