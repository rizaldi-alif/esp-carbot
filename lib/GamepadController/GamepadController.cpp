#include "GamepadController.h"

GamepadController::GamepadController()
{
    // Constructor
}
void GamepadController::begin()
{
    // Initialize Bluepad32
    BP32.setup(&GamepadController::onConnect, &GamepadController::onDisconnect);
}
void GamepadController::update()
{
    // Update Bluepad32 state
    BP32.update();
}
void GamepadController::onConnect(ControllerPtr ctrl)
{
    // Handle controller connection
    Serial.println("Controller connected: " + ctrl->getName());
}
void GamepadController::onDisconnect(ControllerPtr ctrl)
{
    // Handle controller disconnection
    Serial.println("Controller disconnected: " + ctrl->getName());
}