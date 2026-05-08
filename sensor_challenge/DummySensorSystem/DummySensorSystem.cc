#include <string>
#include <unordered_map>
#include <utility>

#include <gz/msgs/stringmsg.pb.h>

#include <gz/common/Profiler.hh>
#include <gz/plugin/Register.hh>
#include <gz/sensors/SensorFactory.hh>

#include <sdf/Sensor.hh>

#include <gz/sim/components/CustomSensor.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/ParentEntity.hh>
#include <gz/sim/components/Sensor.hh>
#include <gz/sim/components/World.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/sim/Util.hh>

#include "DummySensor.hh"
#include "DummySensorSystem.hh"

using namespace custom;

void DummySensorSystem::CreateSensor(gz::sim::Entity _entity,
    const sdf::Sensor &_sdfData,
    const gz::sim::Entity &_parent,
    gz::sim::EntityComponentManager &_ecm)
{
    auto sensorScopedName = gz::sim::removeParentScope(
        gz::sim::scopedName(_entity, _ecm, "::", false), "::");
    sdf::Sensor data = _sdfData;
    data.SetName(sensorScopedName);

    if (data.Topic().empty())
    {
        std::string topic = scopedName(_entity, _ecm) + "/dummy";
        data.SetTopic(topic);
    }

    gz::sensors::SensorFactory sensorFactory;
    auto sensor = sensorFactory.CreateSensor<custom::DummySensor>(data);
    if (nullptr == sensor)
    {
        gzerr << "Failed to create dummy [" << sensorScopedName << "]"
              << std::endl;
        return;
    }

    auto parentName = _ecm.Component<gz::sim::components::Name>(_parent)->Data();
    sensor->SetParent(parentName);

    _ecm.CreateComponent(_entity,
        gz::sim::components::SensorTopic(sensor->Topic()));

    this->entitySensorMap.insert(std::make_pair(_entity, std::move(sensor)));
}

void DummySensorSystem::Configure(const gz::sim::Entity &,
    const std::shared_ptr<const sdf::Element> &,
    gz::sim::EntityComponentManager &_ecm,
    gz::sim::EventManager &)
{
    _ecm.Each<gz::sim::components::CustomSensor,
              gz::sim::components::ParentEntity>(
        [&](const gz::sim::Entity &_entity,
            const gz::sim::components::CustomSensor *_custom,
            const gz::sim::components::ParentEntity *_parent) -> bool
        {
            if (this->entitySensorMap.find(_entity) ==
                this->entitySensorMap.end())
            {
                this->CreateSensor(_entity, _custom->Data(), _parent->Data(),
                                   _ecm);
            }
            return true;
        });
}

void DummySensorSystem::PreUpdate(const gz::sim::UpdateInfo &,
    gz::sim::EntityComponentManager &_ecm)
{
    _ecm.EachNew<gz::sim::components::CustomSensor,
                 gz::sim::components::ParentEntity>(
        [&](const gz::sim::Entity &_entity,
            const gz::sim::components::CustomSensor *_custom,
            const gz::sim::components::ParentEntity *_parent) -> bool
        {
            if (this->entitySensorMap.find(_entity) ==
                this->entitySensorMap.end())
            {
                this->CreateSensor(_entity, _custom->Data(), _parent->Data(),
                                   _ecm);
            }
            return true;
        });
}

void DummySensorSystem::PostUpdate(const gz::sim::UpdateInfo &_info,
    const gz::sim::EntityComponentManager &_ecm)
{
    if(!_info.paused)
    {
        for(auto &[entity, sensor] : this->entitySensorMap)
        {
            sensor->Update(_info.simTime);
        }
    }

    this->RemoveSensorEntities(_ecm);
}

void DummySensorSystem::RemoveSensorEntities(
    const gz::sim::EntityComponentManager &_ecm)
{
    _ecm.EachRemoved<gz::sim::components::CustomSensor>(
        [&](const gz::sim::Entity &_entity,
            const gz::sim::components::CustomSensor *)->bool
        {
            if (this->entitySensorMap.erase(_entity) == 0)
            {
                gzerr << "Error, entity dummy not found ["
                      << _entity << "]" << std::endl;
            }
            return true;
        });
}

GZ_ADD_PLUGIN(DummySensorSystem, gz::sim::System,
    DummySensorSystem::ISystemConfigure,
    DummySensorSystem::ISystemPreUpdate,
    DummySensorSystem::ISystemPostUpdate
)

GZ_ADD_PLUGIN_ALIAS(DummySensorSystem, "custom::DummySensorSystem")
