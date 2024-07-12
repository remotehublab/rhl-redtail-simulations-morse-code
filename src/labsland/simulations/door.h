#ifndef HYBRIDAPI_DOOR_H
#define HYBRIDAPI_DOOR_H

#include "simulation.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

struct DoorData : public BaseOutputDataType
{

    bool open;
    bool close;

    std::string serialize() const
    {
        std::stringstream stream;
        stream << open << "&" << close << "&";
        return stream.str();
    }
};

struct DoorRequest : public BaseInputDataType
{
    bool doorOpened;
    bool doorClosed;
    bool personSensor;

    bool deserialize(std::string const &input)
    {
        std::stringstream stream(input);
        std::string segment;
        std::vector<std::string> segments;

        while (std::getline(stream, segment, '&'))
        {
            segments.push_back(segment);
        }

        if (segments.size() != 3)
        {
            return false;
        }

        if (segments[0] == "1")
            doorOpened = true;
        else if (segments[0] == "0")
            doorOpened = false;
        else
            return false;

        if (segments[1] == "1")
            doorClosed = true;
        else if (segments[1] == "0")
            doorClosed = false;
        else
            return false;

        if (segments[2] == "1")
            personSensor = true;
        else if (segments[2] == "0")
            personSensor = false;
        else
            return false;

        return true;
    }
};

class DoorSimulation : public Simulation<DoorData, DoorRequest>
{

public:
    DoorSimulation() = default;

    virtual void update(double delta) override;

    virtual void initialize() override;
};

#endif // HYBRIDAPI_DOOR_H