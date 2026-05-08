#ifndef DUMMYSENSORSYSTEM_HH_
#define DUMMYSENSORSYSTEM_HH_

#include <gz/sim/System.hh>
#include <gz/sensors/Sensor.hh>
#include <gz/transport/Node.hh>

namespace custom
{
    class DummySensor;

    class DummySensorSystem :
        public gz::sim::System,
        public gz::sim::ISystemConfigure,
        public gz::sim::ISystemPreUpdate,
        public gz::sim::ISystemPostUpdate
    {
    public:
        void Configure(const gz::sim::Entity &_entity,
            const std::shared_ptr<const sdf::Element> &_sdf,
            gz::sim::EntityComponentManager &_ecm,
            gz::sim::EventManager &_eventMgr) final;

        void PreUpdate(const gz::sim::UpdateInfo &_info,
            gz::sim::EntityComponentManager &_ecm) final;

        void PostUpdate(const gz::sim::UpdateInfo &_info,
            const gz::sim::EntityComponentManager &_ecm) final;

    private:
        void CreateSensor(gz::sim::Entity _entity,
            const sdf::Sensor &_sdfData,
            const gz::sim::Entity &_parent,
            gz::sim::EntityComponentManager &_ecm);

        void RemoveSensorEntities(
            const gz::sim::EntityComponentManager &_ecm);

        std::unordered_map<gz::sim::Entity,
            std::shared_ptr<DummySensor>> entitySensorMap;
    };
}

#endif