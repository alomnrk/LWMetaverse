//
// Created by Artem Ustinov on 26.09.2022.
//

#ifndef LWMETAVERSE_SKYBOXRENDERSYSTEM_H
#define LWMETAVERSE_SKYBOXRENDERSYSTEM_H

#include "../vulkan_utils/device.h"
#include "../utils/frame_info.h"
#include "../vulkan_utils/pipeline.h"
#include "../assets/model.h"
#include "../assets/AssetsSystem.h"
#include "../logic/Scene.h"

namespace lwmeta {
    class SkyBoxRenderSystem {
    public:
        SkyBoxRenderSystem(
                Device
        &device,
        VkRenderPass renderPass, VkDescriptorSetLayout
        globalSetLayout, VkDescriptorSetLayout textureSetLayout, AssetsSystem& assetsSystem);

        ~SkyBoxRenderSystem();

        SkyBoxRenderSystem(const SkyBoxRenderSystem &) = delete;

        SkyBoxRenderSystem &operator=(const SkyBoxRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo, GameObject &skybox);

    private:
        AssetsSystem &assetsSystem;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout cubeMapSetLayout);

        void createPipeline(VkRenderPass renderPass);

        void createModel();

        Device &device;

        std::unique_ptr <Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        std::unique_ptr<Model> skyBoxModel;
    };
}

#endif //LWMETAVERSE_SKYBOXRENDERSYSTEM_H
