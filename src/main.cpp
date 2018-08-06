#include <uWS/uWS.h>
#include <iostream>
using namespace std;
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

double best_error = 0.0;

bool decreasedP = false;
bool decreasedI = false;
bool decreasedD = false;
bool increasedP = false;
bool increasedI = false;
bool increasedD = false;

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
  pid.Init(0.1341, 0.00027, 3.0539);

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());

          /*if((pid.errors[0] + pid.errors[1] + pid.errors[2]) <= pid.twiddle_tolerance){
            cout<<pid.Kp<<" "<<pid.Ki<<" "<<pid.Kd<<endl;
            return;
          }*/

          //if(!pid.runComplete){
            pid.UpdateError(cte);
            double steer_value = pid.steer_value;
            // DEBUG
            std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

            json msgJson;
            msgJson["steering_angle"] = steer_value;
            msgJson["throttle"] = 0.3;
            auto msg = "42[\"steer\"," + msgJson.dump() + "]";
            std::cout << msg << std::endl;
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

          /*}else{
             if(pid.twiddle_step == 1){
               best_error = pid.TotalError();
               pid.Kp += pid.errors[0];
               increasedP =  true;
             }else{

               if(!increasedP && !increasedI && !increasedD && !decreasedP  && !decreasedI  && !decreasedD){
                 pid.Kp += pid.errors[0];
                 increasedP =  true;
               }else{
                 double err = pid.TotalError();
                 if(err < best_error){
                      best_error = err;


                      if(increasedD){
                        pid.errors[2] *=1.1;
                        increasedD = false;
                      }

                      if(increasedI){
                        pid.errors[1] *=1.1;
                        pid.Kd += pid.errors[2];
                        increasedI =  false;
                        increasedD = true;
                      }

                      if(increasedP){
                        pid.errors[0] *=1.1;
                        pid.Ki += pid.errors[1];
                        increasedP =  false;
                        increasedI = true;
                      }

                      if(decreasedD){
                        pid.errors[2] *=1.1;
                        decreasedD = false;
                      }

                      if(decreasedI){
                        pid.errors[1] *=1.1;
                        decreasedI =  false;
                      }

                      if(decreasedP){
                        pid.errors[0] *=1.1;
                        decreasedP =  false;
                      }



                 }else{
                   if(increasedP){
                     pid.Kp -= 2*pid.errors[0];
                     decreasedP =  true;
                   }else if(increasedI){
                     pid.Ki -= 2*pid.errors[1];
                     decreasedI = true;
                   }else if(increasedD){
                     pid.Kd -= 2*pid.errors[2];
                     decreasedD = true;
                   }

                   if(decreasedP){
                     pid.Kp +=pid. errors[0];
                     pid.errors[0] *=0.9;
                   }else if(decreasedI){
                     pid.Ki += pid.errors[1];
                     pid.errors[1] *=0.9;
                   }else if(decreasedD){
                     pid.Kd += pid.errors[2];
                     pid.errors[2] *=0.9;
                   }
                 }

               }
               }


*/

          }
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */


      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
