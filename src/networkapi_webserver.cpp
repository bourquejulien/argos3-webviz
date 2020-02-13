#include "networkapi_webserver.h"

#include "helpers/utils.h"

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {
  namespace NetworkAPI {

    /****************************************/
    /****************************************/

    CWebServer::CWebServer()
        : m_vecWebThreads(std::thread::hardware_concurrency()) {}

    /****************************************/
    /****************************************/

    void CWebServer::Start() {
      try {
        LOG_S(INFO) << "Starting " << m_vecWebThreads.size()
                    << " threads for WebServer";

        /* Start Webserver */
        std::transform(
          m_vecWebThreads.begin(),
          m_vecWebThreads.end(),
          m_vecWebThreads.begin(),
          [&](std::thread *t) {
            return new std::thread([&]() {
              auto cMyApp = uWS::App();

              this->SetupWebApp(cMyApp);
              /* Start listening to Port */
              cMyApp
                .listen(
                  m_unPort,
                  [&](auto *pc_token) {
                    if (pc_token) {
                      LOG_S(INFO) << "Thread listening on port " << m_unPort;

                      /* Set experiment state */
                      // m_eExperimentState = EXPERIMENT_INITIALIZED;
                    } else {
                      ABORT_F(
                        "WebServer::Execute() failed to listen on port "
                        "%d",
                        m_unPort);
                      return;
                    }
                  })
                .run();
            });
          });

        /* Main cycle */
        // while (!m_cSimulator.IsExperimentFinished()) {
        //   RealTimeStep();
        // }
        // /* The experiment is finished */
        // m_cSimulator.GetLoopFunctions().PostExperiment();

        std::for_each(
          m_vecWebThreads.begin(), m_vecWebThreads.end(), [](std::thread *t) {
            t->join();
          });
      } catch (CARGoSException &ex) {
        THROW_ARGOSEXCEPTION_NESTED(
          "Error while executing the experiment.", ex);
      }
    }

    /****************************************/
    /****************************************/

    void CWebServer::SetupWebApp(uWS::App &c_MyApp) {
      /* Setup WebSockets */
      c_MyApp.ws<m_sPerSocketData>(
        "/*",
        {/* Settings */
         .compression = uWS::SHARED_COMPRESSOR,
         .maxPayloadLength = 16 * 1024 * 1024,
         .idleTimeout = 10,
         .maxBackpressure = 1 * 1024 * 1204,
         /* Handlers */
         .open =
           [&](auto *pc_ws, uWS::HttpRequest *pc_req) {
             /* Selectivly subscribe to different channels */
             if (pc_req->getQuery().size() > 0) {
               std::vector<std::string_view> vecQueries =
                 SplitSV(pc_req->getQuery(), ",");

               std::for_each(
                 vecQueries.begin(),
                 vecQueries.end(),
                 [pc_ws](std::string_view strv_channel) {
                   pc_ws->subscribe(strv_channel);
                 });
             } else {
               /*
                * making every connection subscribe to the
                * "broadcast" and "events" topics
                */
               pc_ws->subscribe("broadcast");
               pc_ws->subscribe("events");
             }

             /*
              * Add to list of clients connected
              */
             m_vecWebSocketClients.push_back(pc_ws);
           },
         .message =
           [](
             auto *pc_ws, std::string_view strv_message, uWS::OpCode e_opCode) {
             /* broadcast every single message it got */
             pc_ws->publish("broadcast", strv_message, e_opCode);
           },
         .close =
           [&](auto *pc_ws, int n_code, std::string_view strv_message) {
             /* it automatically unsubscribe from any topic here */

             /*
              * Remove from the list of all clients connected
              */
             EraseFromVector(m_vecWebSocketClients, pc_ws);
           }});

      /* Setup routes */
      c_MyApp.get("/start", [&](auto *pc_res, auto *pc_req) {
        nlohmann::json cMyJson;
        cMyJson["status"] = "Started Playing";
        this->SendJSON(pc_res, cMyJson);
      });
    }

    /****************************************/
    /****************************************/

    void CWebServer::SendJSON(
      uWS::HttpResponse<false> *pc_res, nlohmann::json c_json_data) {
      pc_res->cork([&]() {
        pc_res->writeHeader("Access-Control-Allow-Origin", "*");
        pc_res->writeHeader("Content-Type", "application/json");
        pc_res->end(c_json_data.dump());
      });
    }

    /****************************************/
    /****************************************/

    void CWebServer::EmitEvent(std::string_view event_name) {
      std::stringstream strJson;

      // // TODO : Build a JSON to string marshal function
      // strJson << "{";
      // strJson << "\"event\":\"";
      // strJson << event_name;
      // strJson << "\",\"state\":\"";
      // strJson << EExperimentStateToString(this->m_eExperimentState);
      // strJson << "\",\"isFastForward\":";
      // strJson << (this->m_bFastForwarding ? "true" : "false");
      // strJson << "}";

      // std::string strJs = strJson.str();

      // std::for_each(
      //   m_vecWebSocketClients.begin(),
      //   m_vecWebSocketClients.end(),
      //   [strJs](auto *ws) {
      //     //                                            Compress = true
      //     ws->publish("events", strJs, uWS::OpCode::TEXT, true);
      //   });
    }

    /****************************************/
    /****************************************/

    void CWebServer::BroadcastState() {
      // std::stringstream strJson;

      // // TODO : Build a JSON to string marshal function
      // strJson << "{";
      // strJson << "\"state\":\"";
      // strJson << EExperimentStateToString(this->m_eExperimentState);
      // strJson << "\",\"isFastForward\":";
      // strJson << (this->m_bFastForwarding ? "true" : "false");
      // strJson << "}";

      // std::string strJs = strJson.str();

      // std::for_each(
      //   m_vecWebSocketClients.begin(),
      //   m_vecWebSocketClients.end(),
      //   [strJs](auto *ws) {
      //     try {
      //       //                                            Compress = true
      //       ws->publish("broadcast", strJs, uWS::OpCode::TEXT, true);
      //     } catch (const std::exception &e) {
      //       LOG_S(ERROR) << e.what() << '\n';
      //     }
      //   });
    }

    /****************************************/
    /****************************************/

    void CWebServer::BroadcastMessage(std::string_view message) {
      std::for_each(
        m_vecWebSocketClients.begin(),
        m_vecWebSocketClients.end(),
        [message](auto *ws) {
          //                                            Compress = true
          ws->publish("broadcast", message, uWS::OpCode::TEXT, true);
        });
    }

  }  // namespace NetworkAPI

}  // namespace argos