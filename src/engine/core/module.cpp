#include "module.hpp"

namespace engine::core {
    void ModuleManager::init()
    {
        for (const auto& [_, module] : m_modules) {
            for (const auto& requireType : module->m_require) {
                module->m_moduleManager = this;

                const auto found = m_modules.equal_range(requireType);
                for (auto it = found.first; it != found.second; it++) {
                    it->second->m_inNeighbours.push_back(
                        std::static_pointer_cast<utils::Node>(it->second));
                    module->m_outNeighbours.push_back(
                        std::static_pointer_cast<utils::Node>(it->second));
                }
            }
        }

        utils::Graph graph;

        for (const auto& [_, module] : m_modules) {
            graph.addNode(std::static_pointer_cast<utils::Node>(module));
        }

        graph.sort();

        for (const auto& node : graph.getNodes()) {
            auto module = std::reinterpret_pointer_cast<Module>(node);
            for (const auto& stage : module->m_stages) {
                m_stages.insert(std::pair(stage.m_stage, stage));
            }
        }
    }

    void ModuleManager::callStage(ModuleLifecycle stage)
    {
        auto found = m_stages.equal_range(stage);
        for (auto it = found.first; it != found.second; it++) {
            it->second.m_function();
        }
    }
} // namespace engine::core