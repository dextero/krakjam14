#pragma once

#include <map>
#include <string>

template<typename T>
class ResourceManager
{
public:
    ~ResourceManager()
    {
    }

    T& Get(const std::string& name)
    {
        auto it = resources.find(name);
        if (it == resources.end())
        {
            T elem;
            elem.LoadFromFile(name);

            resources.insert(std::pair<std::string, T>(name, elem));
        }

        return resources[name];
    }

    static ResourceManager& Get()
    {
        if (singleton == NULL)
            singleton = new ResourceManager();

        return *singleton;
    }

private:
    ResourceManager()
    {
    }

    std::map<std::string, T> resources;

    static ResourceManager* singleton;
};