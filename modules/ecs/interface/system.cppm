module;

#include <string>
#include <typeinfo>

export module ecs:system;

import :isystem;
import :family_type_id;
import :system_manager;

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

    [[nodiscard]] std::string getSystemTypeName() const override
    {
        static std::string SYSTEM_TYPE_NAME{typeid(T).name()};
        return SYSTEM_TYPE_NAME;
    }

    template<typename... Dependencies>
    void addDependencies(Dependencies&&... dependencies)
    {
        this->system_manager->addSystemDependency(this, std::forward<Dependencies>(dependencies)...);
    }

    virtual void preUpdate(float dt) override {}
    virtual void update(float dt) override {}
    virtual void postUpdate(float dt) override {}

protected:
    System() {}

private:
    friend class SystemManager;

    SystemManager* system_manager;
};

template<typename T>
const SystemTypeId System<T>::STATIC_SYSTEM_TYPE_ID = util::FamilyTypeId<ISystem>::get<T>();

} // namespace gg;
