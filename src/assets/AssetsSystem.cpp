//
// Created by Artem Ustinov on 27.09.2022.
//

#include "AssetsSystem.h"

namespace lwmeta {
    AssetsSystem::AssetsSystem(Device &device) : device{device} {}

    id_t AssetsSystem::AddTexture(const std::string filepath, uint32_t desired_channels, bool gammaCorrection) {
        Texture *texture = Texture::createTexture(device, filepath, desired_channels, gammaCorrection);
        textures.emplace(texture->getId(), texture);
        return texture->getId();
    }

    id_t AssetsSystem::AddCubeMap(const std::vector<char *> &fileNames) {
        CubeMap *cubeMap = CubeMap::createTexture(device, fileNames);
        cubeMaps.emplace(cubeMap->getId(), cubeMap);
        return cubeMap->getId();
    }

    id_t AssetsSystem::AddModel(Model *model) {
        models.emplace(currentId++, model);
        return currentId - 1;
    }

    Model* AssetsSystem::GetModel(id_t modelId){
        return models.at(modelId);
    }

//    id_t AssetsSystem::CreateTextureMaterial(uint32_t texture_id, uint32_t binding, DescriptorSetLayout *layout,
//                                             DescriptorPool *pool) {
//        auto material = new TextureMaterial(currentId++, textures.at(texture_id), binding, layout, pool);
//        materials.emplace(material->getId(), material);
//        return material->getId();
//    }
//
//    id_t AssetsSystem::CreateCubeMapMaterial(uint32_t cubeMap_id, DescriptorSetLayout *layout, DescriptorPool *pool) {
//        auto material = new SkyBoxMaterial(currentId++, cubeMaps.at(cubeMap_id), layout, pool);
//        materials.emplace(material->getId(), material);
//        return material->getId();
//    }

//    id_t AssetsSystem::CreateBlackHoleMaterial(id_t cubeMap_id, id_t disktexture, DescriptorSetLayout *layout,
//                                               DescriptorPool *pool) {
//        auto material = new BlackHoleMaterial(currentId++, cubeMaps.at(cubeMap_id), textures.at(disktexture), layout,
//                                              pool);
//        materials.emplace(material->getId(), material);
//        return material->getId();
//    }
//
//    id_t AssetsSystem::CreateLitMaterial(id_t texture, DescriptorSetLayout *layout,
//                                               DescriptorPool *pool) {
//        auto material = new LitMaterial(currentId++, textures.at(texture), layout,
//                                              pool);
//        materials.emplace(material->getId(), material);
//        return material->getId();
//    }

//    id_t AssetsSystem::CreateDissolveLitMaterial(id_t texture, DescriptorSetLayout *layout,
//                                         DescriptorPool *pool) {
//        auto material = new DissolveLitMaterial(device, currentId++, textures.at(texture), layout,
//                                        pool);
//        materials.emplace(material->getId(), material);
//        return material->getId();
//    }
//
//    Material *AssetsSystem::GetMaterial(lwmeta::AssetsSystem::id_t material_id) {
//        return materials.at(material_id);
//    }

    AssetsSystem::~AssetsSystem() {
//        for (auto &kv: materials) {
//            auto &material = kv.second;
//            delete material;
//        }

        for (auto &kv: textures) {
            auto &texture = kv.second;
            delete texture;
        }

        for (auto &kv: cubeMaps) {
            auto &cubeMap = kv.second;
            delete cubeMap;
        }

        for (auto &kv: models) {
            auto &model = kv.second;
            delete model;
        }
    }
}