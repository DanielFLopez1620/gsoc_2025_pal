#include <chrono>

#include <gz/msgs/stringmsg.pb.h>

#include <gz/common/Console.hh>
#include <gz/msgs/Utility.hh>
#include <gz/sensors/Util.hh>

#include "DummySensor.hh"

using namespace custom;

bool DummySensor::Load(const sdf::Sensor &_sdf)
{
    auto type = gz::sensors::customType(_sdf);
    if("dummy" != type)
    {
        gzerr << "Trying to load [dummy] sensor, but got invalid ["
              << type << "]." << std::endl;
        return false;
    }

    gz::sensors::Sensor::Load(_sdf);

    this->_pub = this->_node.Advertise<gz::msgs::StringMsg>(this->Topic());

    this->concatenatePayload(" in frame " + this->Name());

    if (!_sdf.Element()->HasElement("gz:dummy"))
    {
        gzdbg << "No custom config for [" << this->Topic() << "]"
              << std::endl;
        return true;
    }

    this->concatenatePayload( " with additional info " +
            _sdf.Element()->Get<std::string>("gz:dummy"));

    return true;
}

bool DummySensor::Update(const std::chrono::steady_clock::duration &_now)
{
    gz::msgs::StringMsg msg;
    *msg.mutable_header()->mutable_stamp() = gz::msgs::Convert(_now);
    auto frame = msg.mutable_header()->add_data();
    frame->set_key("frame_id");

    msg.set_data(this->getPayload());

    this->AddSequence(msg.mutable_header());
    this->_pub.Publish(msg);

    return true;
}

std::string DummySensor::getPayload()
{
    return this->_payload;
}

void DummySensor::setPayload(const std::string& payload)
{
    this->_payload = payload;
}

void DummySensor::concatenatePayload(const std::string& add_payload)
{
    this->_payload += add_payload;
}