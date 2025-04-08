#ifndef DUMMY_SENSOR_HH_
#define DUMMY_SENSOR_HH_

#include <gz/sensors/Sensor.hh>
#include <gz/sensors/SensorTypes.hh>
#include <gz/transport/Node.hh>

namespace custom
{
    class DummySensor : public gz::sensors::Sensor
    {
    public:
        virtual bool Load(const sdf::Sensor &_sdf) override;

        virtual bool Update(const std::chrono::steady_clock::duration &_now) override;

        std::string getPayload();

        void setPayload(const std::string& payload);

        void concatenatePayload(const std::string& add_payload);
    private:
        std::string _payload {"DummySensor"};

        gz::transport::Node _node;

        gz::transport::Node::Publisher _pub;
    };
}

#endif