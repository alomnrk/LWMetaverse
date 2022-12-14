#pragma once

#include "../utils/camera.h"
#include "../vulkan_utils/device.h"
#include "../utils/frame_info.h"
#include "../logic/game_object.h"
#include "../vulkan_utils/pipeline.h"

// std
#include <memory>
#include <vector>

namespace lwmeta {
class PointLightSystem {
 public:
  PointLightSystem(
          Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo &frameInfo, GlobalUbo &ubo, GameObject::Map &gameObjects);
  void render(FrameInfo &frameInfo, GameObject::Map &gameObjects);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  Device &device;

  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace lwmeta
