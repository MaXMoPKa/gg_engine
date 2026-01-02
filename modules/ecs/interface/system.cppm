module;

#include <typeinfo>
#include <utility>

export module ecs.system;

import ecs.isystem;
import ecs.family_type_id;
import ecs.system_manager;
import ecs.common;

import types.string;
import types.base_types;

namespace gg
{
    
export template<typename T>
class System : public ISystem
{
public:
    virtual ~System() {}

    static const SystemTypeId STATIC_SYSTEM_TYPE_ID;

    [[nodiscard]] virtual const SystemTypeId getStaticSystemTypeId() const
    {
        return STATIC_SYSTEM_TYPE_ID;
    }

    [[nodiscard]] String getSystemTypeName() const override
    {
        static String SYSTEM_TYPE_NAME{typeid(T).name()};
        return SYSTEM_TYPE_NAME;
    }

    template<typename... Dependencies>
    void addDependencies(Dependencies&&... dependencies)
    {
        this->system_manager->addSystemDependency(this, std::forward<Dependencies>(dependencies)...);
    }

    virtual void preUpdate(F32 dt) override {}
    virtual void update(F32 dt) override {}
    virtual void postUpdate(F32 dt) override {}

protected:
    System() {}

private:
    friend class SystemManager;
        
    SystemManager* system_manager;
};

template<typename T>
const SystemTypeId System<T>::STATIC_SYSTEM_TYPE_ID = util::FamilyTypeId<ISystem>::get<T>();

} // namespace gg;
