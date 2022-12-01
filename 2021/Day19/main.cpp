#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <assert.h>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    struct Beacon
    {
        Beacon(const std::string& input)
        {
            const auto firstComma = input.find(",");
            const auto secondComma = input.find(",", firstComma + 1);
            x = std::atoi(input.substr(0, firstComma).c_str());
            y = std::atoi(input.substr(firstComma + 1, secondComma - firstComma).c_str());
            z = std::atoi(input.substr(secondComma + 1).c_str());
        }

        Beacon(int32_t xIn, int32_t yIn, int32_t zIn):
            x(xIn), y(yIn), z(zIn)
        {}

        void Adjust(const Beacon& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        }

        std::string ToString() const
        {
            std::ostringstream outStream;
            outStream << x << "," << y << "," << z;
            return outStream.str();
        }

        int32_t x{};
        int32_t y{};
        int32_t z{};
    };

    bool operator==(const Beacon& left, const Beacon& right)
    {
        return (left.x == right.x) &&
               (left.y == right.y) &&
               (left.z == right.z);
    }

    bool operator<(const Beacon& left, const Beacon& right)
    {
        if (left.x != right.x)
        {
            return left.x < right.x;
        }
        else if (left.y != right.y)
        {
            return left.y < right.y;
        }
        return left.z < right.z;
    }

    struct Scanner
    {
        Scanner(const std::string& header, std::istream& inStream)
        {
            auto idString = header.substr(12, 2);
            id = std::atoi(idString.c_str());

            std::string input;
            while (std::getline(inStream, input))
            {
                if (input.empty()) return;

                beacons.emplace_back(Beacon{input});
            }
        }

        Scanner(int32_t idIn, std::vector<Beacon> beaconsIn):
            id(idIn), beacons(std::move(beaconsIn))
        {}
        
        std::optional<Beacon> TryMatchInner(const Beacon& myBeacon, const Beacon& otherBeacon, const Scanner& other, uint32_t threshold)
        {
            Beacon otherScannerLocationRelativeToMe{ myBeacon.x + (-1 * otherBeacon.x), myBeacon.y + (-1 * otherBeacon.y), myBeacon.z + (-1 * otherBeacon.z)};
            uint32_t matches{};
            for (auto otherBeaconInner : other.beacons)
            {
                otherBeaconInner.Adjust(otherScannerLocationRelativeToMe);
                if (std::find(beacons.begin(), beacons.end(), otherBeaconInner) != beacons.end())
                {
                    ++matches;
                }
            }

            if (matches >= threshold)
            {
                return otherScannerLocationRelativeToMe;
            }
            return{};
        }

        std::optional<Beacon> TryMatch(const Scanner& other, uint32_t threshold)
        {
            // N^2 try each beacon and assume they are a match between the scanners.  If they line up >threshold then
            // we have a true match.  Otherwise try the next pair and hope for the best.
            for(const auto& beacon : beacons)
            {
                for(const auto& otherBeacon : other.beacons)
                {
                    const auto match = TryMatchInner(beacon, otherBeacon, other, threshold);
                    if (match.has_value())
                    {
                        return match;
                    }
                }
            }
            return {};
        }

        int32_t id{};
        std::vector<Beacon> beacons;
    };

    std::vector<Scanner> ParseInput(std::istream& inStream)
    {
        std::vector<Scanner> scanners;

        std::string input;
        while (std::getline(inStream, input))
        {
            scanners.emplace_back(Scanner{input, inStream});
        }
        return scanners;
    }

    void TEST()
    {
        assert((Beacon{"404,-588,-901"} == Beacon{404, -588, -901}));
        assert((Beacon{"528,-643,409"} == Beacon{528, -643, 409}));

        // --- scanner 0 ---
        // 0,2
        // 4,1
        // 3,3
        std::vector<Beacon> beaconZero = {
            {"0,2,0"},
            {"4,1,0"},
            {"3,3,0"}
        };
        Scanner zero{ 0, std::move(beaconZero) };

        // --- scanner 1 ---
        // -1,-1
        // -5,0
        // -2,1
        std::vector<Beacon> beaconOne = {
            {"-1,-1,0"},
            {"-5,0,0"},
            {"-2,1,0"}
        };
        Scanner one{ 1, std::move(beaconOne) };

        const auto match = zero.TryMatch(one, 3UL);
        assert((match == Beacon{5, 2, 0}));

        std::string sampleInput = R"(--- scanner 0 ---
404,-588,-901
528,-643,409
-838,591,734
390,-675,-793
-537,-823,-458
-485,-357,347
-345,-311,381
-661,-816,-575
-876,649,763
-618,-824,-621
553,345,-567
474,580,667
-447,-329,318
-584,868,-557
544,-627,-890
564,392,-477
455,729,728
-892,524,684
-689,845,-530
423,-701,434
7,-33,-71
630,319,-379
443,580,662
-789,900,-551
459,-707,401

--- scanner 1 ---
686,422,578
605,423,415
515,917,-361
-336,658,858
95,138,22
-476,619,847
-340,-569,-846
567,-361,727
-460,603,-452
669,-402,600
729,430,532
-500,-761,534
-322,571,750
-466,-666,-811
-429,-592,574
-355,545,-477
703,-491,-529
-328,-685,520
413,935,-424
-391,539,-444
586,-435,557
-364,-763,-893
807,-499,-711
755,-354,-619
553,889,-390
)";
        std::istringstream inStream{ sampleInput };
        auto scanners = ParseInput(inStream);

        assert(scanners[0].TryMatch(scanners[1], 12UL).has_value());
    }

    void Part1()
    {
        auto scanners = ParseInput(std::cin);

        Scanner world{99, scanners[0].beacons};

        std::vector<int32_t> matchedScanners { scanners[0].id };

        uint32_t loopCount{};
        while (matchedScanners.size() != scanners.size())
        {
            if (++loopCount > 1000)
            {
                std::cerr << "1000 iterations without reaching closure.  Aborting" << std::endl;
                std::cout << -1 << std::endl;
                return;
            }

            for (const auto& scanner : scanners)
            {
                if (std::find(matchedScanners.begin(), matchedScanners.end(), scanner.id) != matchedScanners.end()) continue; // already matched

                const auto match = world.TryMatch(scanner, 12UL);
                if (match.has_value())
                {
                    for (auto beacon : scanner.beacons)
                    {
                        beacon.Adjust(match.value());
                        if (std::find(world.beacons.begin(), world.beacons.end(), beacon) != world.beacons.end())
                        {
                            world.beacons.push_back(beacon);
                        }

                        // Add to the matched list so we stop checking
                        matchedScanners.push_back(scanner.id);
                    }
                }
            }
        }

        std::sort(world.beacons.begin(), world.beacons.end());
        for (const auto& beacon : world.beacons)
        {
            std::cerr << beacon.ToString() << std::endl;
        }

        std::cout << world.beacons.size() << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    TEST();
    RunPart1() ? Part1() : Part2();
    return 0;
}