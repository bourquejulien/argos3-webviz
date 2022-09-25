/**
 * @file
 * <argos3/plugins/simulator/visualizations/webviz/entity/webviz_crazyflie.cpp>
 *
 * @author Guillaume Ricard - <guillaume.ricard@polymtl.ca>
 *
 * @project ARGoS3-Webviz <https://github.com/NESTLab/argos3-webviz>
 *
 * MIT License
 * Copyright (c) 2022 MIST Lab
 */

#include <argos3/plugins/robots/crazyflie/simulator/crazyflie_entity.h>
#include <argos3/plugins/simulator/visualizations/webviz/webviz.h>

#include <nlohmann/json.hpp>

namespace argos {
  namespace Webviz {

    // cppcheck suppress noConstructor
    class CWebvizOperationGenerateCrazyflieJSON
        : public CWebvizOperationGenerateJSON {
     public:
      /**
       * @brief Function called to generate a JSON representation of Crazyflie
       *
       * @param c_webviz
       * @param c_entity
       * @return nlohmann::json
       */
      nlohmann::json ApplyTo(CWebviz &c_webviz, CCrazyflieEntity &c_entity) {
        nlohmann::json cJson;

        cJson["type"] = c_entity.GetTypeDescription();
        cJson["id"] = c_entity.GetId();

        /* Get the position of the crazyflie */
        const argos::CVector3 &cPosition =
          c_entity.GetEmbodiedEntity().GetOriginAnchor().Position;

        /* Add it to json as => position:{x, y, z} */
        cJson["position"]["x"] = cPosition.GetX();
        cJson["position"]["y"] = cPosition.GetY();
        cJson["position"]["z"] = cPosition.GetZ();

        /* Get the orientation of the crazyfie */
        const argos::CQuaternion cOrientation =
          c_entity.GetEmbodiedEntity().GetOriginAnchor().Orientation;

        cJson["orientation"]["x"] = cOrientation.GetX();
        cJson["orientation"]["y"] = cOrientation.GetY();
        cJson["orientation"]["z"] = cOrientation.GetZ();
        cJson["orientation"]["w"] = cOrientation.GetW();

        return cJson;
      }
    };

    REGISTER_WEBVIZ_ENTITY_OPERATION(
      CWebvizOperationGenerateJSON,
      CWebvizOperationGenerateCrazyflieJSON,
      CCrazyflieEntity);

  }  // namespace Webviz
}  // namespace argos
