//
// Created by Artem Ustinov on 26.09.2022.
//

#ifndef LWMETAVERSE_SKYBOXRENDERSYSTEM_H
#define LWMETAVERSE_SKYBOXRENDERSYSTEM_H

#include "../device.h"
#include "../frame_info.h"
#include "../pipeline.h"
#include "../model.h"

namespace lwmeta {
    class SkyBoxRenderSystem {
    public:
        SkyBoxRenderSystem(
                Device
        &device,
        VkRenderPass renderPass, VkDescriptorSetLayout
        globalSetLayout);

        ~SkyBoxRenderSystem();

        SkyBoxRenderSystem(const SkyBoxRenderSystem &) = delete;

        SkyBoxRenderSystem &operator=(const SkyBoxRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        void createModel();

        Device &device;

        std::unique_ptr <Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        std::unique_ptr<Model> skyBoxModel;
    };
}

#endif //LWMETAVERSE_SKYBOXRENDERSYSTEM_H
