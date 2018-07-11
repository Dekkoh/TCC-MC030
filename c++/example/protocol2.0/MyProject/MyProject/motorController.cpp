#if defined(__linux__) || defined(__APPLE__)
#include <fcntl.h>
#include <termios.h>
#define STDIN_FILENO 0
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>

#include "dynamixel_sdk.h"                                  // Uses Dynamixel SDK library
#include "motorController.h"

// Control table address
#define ADDR_FIRMIWARE_VERSION          6
#define ADDR_TORQUE_ENABLE              64                 // Control table address is different in Dynamixel model
#define ADDR_LED                        65
#define ADDR_GOAL_PWM					100
#define ADDR_PROFILE_ACCEL              108
#define ADDR_PROFILE_VELOCITY           112
#define ADDR_GOAL_POSITION              116
#define ADDR_MOVING_STATUS              122
#define ADDR_PRESENT_PWM                124
#define ADDR_PRESENT_CURRENT            126
#define ADDR_PRESENT_VELOCITY           128
#define ADDR_PRESENT_POSITION           132
#define ADDR_PRESENT_INPUT_VOLTAGE      144
#define ADDR_PRESENT_TEMPERATURE        146


// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define BAUDRATE                        57600

#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

std::map<std::string, dynamixel::PortHandler*> portMap;

int index = 0;
//int dxl_comm_result = COMM_TX_FAIL;             // Communication result


/************************************************************/
/*                                                          */
/*                     Get functions                        */
/*                                                          */
/************************************************************/

int get_present_position(int DXL_ID, std::string deviceName) {
	int32_t dxl_present_position;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	//dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
	dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_POSITION, (uint32_t*)&dxl_present_position, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_position;
	}

	return (int)dxl_present_position;
}

int get_firmware_version(int DXL_ID, std::string deviceName) {
	uint8_t dxl_firmware_version;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, DXL_ID, ADDR_FIRMIWARE_VERSION, (uint8_t*)&dxl_firmware_version, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_firmware_version;
	}

	return (int)dxl_firmware_version;
}

int get_led_status(int DXL_ID, std::string deviceName) {
	uint8_t dxl_led_status;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, DXL_ID, ADDR_LED, (uint8_t*)&dxl_led_status, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_led_status;
	}

	return (int)dxl_led_status;
}

int get_present_current(int DXL_ID, std::string deviceName) {
	int16_t dxl_present_current;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_CURRENT, (uint16_t*)&dxl_present_current, &dxl_error);

	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_current;
	}
	return (int)dxl_present_current;
}

int get_present_velocity(int DXL_ID, std::string deviceName) {
	uint32_t dxl_present_velocity;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_VELOCITY, (uint32_t*)&dxl_present_velocity, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_velocity;
	}

	return (int)dxl_present_velocity;
}

int get_input_voltage(int DXL_ID, std::string deviceName) {
	uint16_t dxl_input_voltage;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_INPUT_VOLTAGE, (uint16_t*)&dxl_input_voltage, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_input_voltage;
	}

	return (int)dxl_input_voltage;
}

int get_present_temperature(int DXL_ID, std::string deviceName) {
	uint8_t dxl_present_temperature;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_TEMPERATURE, (uint8_t*)&dxl_present_temperature, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_temperature;
	}

	return (int)dxl_present_temperature;
}

int get_present_torque(int DXL_ID, std::string deviceName) {
	uint8_t dxl_present_torque;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, DXL_ID, ADDR_TORQUE_ENABLE, (uint8_t*)&dxl_present_torque, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_torque;
	}

	return (int)dxl_present_torque;
}

int get_profile_acceleration(int DXL_ID, std::string deviceName) {
	uint32_t dxl_profile_acceleration;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PROFILE_ACCEL, (uint32_t*)&dxl_profile_acceleration, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_profile_acceleration;
	}

	return (int)dxl_profile_acceleration;
}

int get_profile_velocity(int DXL_ID, std::string deviceName) {
	uint32_t dxl_profile_velocity;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_ID, ADDR_PROFILE_VELOCITY, (uint32_t*)&dxl_profile_velocity, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_profile_velocity;
	}

	return (int)dxl_profile_velocity;
}

int get_moving_status(int DXL_ID, std::string deviceName) {
	uint8_t dxl_moving_status;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, DXL_ID, ADDR_MOVING_STATUS, (uint8_t*)&dxl_moving_status, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_moving_status;
	}

	return (int)dxl_moving_status;
}

int get_present_pwm(int DXL_ID, std::string deviceName) {
	uint16_t dxl_present_pwm;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_PRESENT_PWM, (uint16_t*)&dxl_present_pwm, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return (int)dxl_present_pwm;
	}

	return (int)dxl_present_pwm;
}

int ping(int DXL_ID, std::string deviceName) {
	uint16_t dxl_model_number;
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return -1;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->ping(portHandler, DXL_ID, &dxl_model_number, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		return -1;
	} else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		return dxl_model_number;
	}

	printf("[ID:%03d] ping Succeeded. Dynamixel model number : %d\n", DXL_ID, dxl_model_number);
	return (int)dxl_model_number;
}

/************************************************************/
/*                                                          */
/*                     Set functions                        */
/*                                                          */
/************************************************************/

void move_to(int DXL_ID, std::string deviceName, int goal_position) {
	bool finished = false;

	while (!finished) {
		int32_t dxl_present_position = 0;               // Present position
		uint8_t dxl_error = 0;
		dynamixel::PortHandler *portHandler;
		dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
		std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

		if (it == portMap.end()) {
			continue;
		}
		portHandler = it->second;

		int dxl_comm_result = COMM_TX_FAIL;
		dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_GOAL_POSITION, goal_position, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
			continue;
		}
		else if (dxl_error != 0 && dxl_error != 128) {
			printf("%s\n", packetHandler->getRxPacketError(dxl_error));
			continue;
		}

		finished = true;
	}

}

void set_led_status(int DXL_ID, std::string deviceName, int led_status) {
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);
	uint8_t dxl_error = 0;

	if (it == portMap.end()) {
		return;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_LED, led_status, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
	}
}

void set_torque(int DXL_ID, std::string deviceName, int torque_status) {
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	
	while (true) {
		std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);
		uint8_t dxl_error = 0;

		if (it == portMap.end()) {
			continue;
		}
		portHandler = it->second;

		int dxl_comm_result = COMM_TX_FAIL;

		char buffer[32];
		_itoa_s(torque_status, buffer, 32, 10);
		LPCSTR p = buffer;

		dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_TORQUE_ENABLE, (uint8_t)torque_status, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
			continue;
		}
		else if (dxl_error != 0 && dxl_error != 128) {
			//char buffer[32];
			//_itoa_s(dxl_error, buffer, 32, 10);
			//LPCSTR p = buffer;

			//OutputDebugString(p);
			//OutputDebugString("\n");
			printf("%s\n", packetHandler->getRxPacketError(dxl_error));
			continue;
		}

		break;
	}
}

void set_profile_acceleration(int DXL_ID, std::string deviceName, int profile_acceleration) {
	while (true) {
		uint8_t dxl_error = 0;
		dynamixel::PortHandler *portHandler;
		dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
		std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

		if (it == portMap.end()) {
			continue;
		}
		portHandler = it->second;

		int dxl_comm_result = COMM_TX_FAIL;
		dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PROFILE_ACCEL, profile_acceleration, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
			continue;
		}
		else if (dxl_error != 0) {
			printf("%s\n", packetHandler->getRxPacketError(dxl_error));
			continue;
		}

		return;
	}

}

void set_profile_velocity(int DXL_ID, std::string deviceName, int profile_velocity) {
	while (true) {
		uint8_t dxl_error = 0;
		dynamixel::PortHandler *portHandler;
		dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
		std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

		if (it == portMap.end()) {
			continue;
		}
		portHandler = it->second;

		int dxl_comm_result = COMM_TX_FAIL;
		dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_PROFILE_VELOCITY, profile_velocity, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
			continue;
		}
		else if (dxl_error != 0 && dxl_error != 128) {
			printf("%s\n", packetHandler->getRxPacketError(dxl_error));
			continue;
		}

		return;
	}
}

void set_goal_pwm(int DXL_ID, std::string deviceName, int goal_pwm) {
	uint8_t dxl_error = 0;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);

	if (it == portMap.end()) {
		return;
	}
	portHandler = it->second;

	int dxl_comm_result = COMM_TX_FAIL;
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_GOAL_PWM, goal_pwm, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
	}
	else if (dxl_error != 0) {
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
	}
}

/************************************************************/
/*                                                          */
/*              Initializer & Deinitializer                 */
/*                                                          */
/************************************************************/

int initialize(std::string deviceName) {
	uint8_t dxl_error = 0;
	std::map<std::string, dynamixel::PortHandler*>::iterator it = portMap.find(deviceName);
	if (it != portMap.end()) {
		// Port already initialized
		return -1;
	}

	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(deviceName.c_str());
	portMap[deviceName] = portHandler;

	// Open port
	if (portMap[deviceName]->openPort()) {
		printf("Succeeded to open the port!\n");
	} else {
		printf("Failed to open the port!\n");
		//printf("Press any key to terminate...\n");
		//getch();
		return -1;
	}

	// Set port baudrate
	if (portMap[deviceName]->setBaudRate(BAUDRATE)) {
		printf("Succeeded to change the baudrate!\n");
	}
	else {
		printf("Failed to change the baudrate!\n");
		//printf("Press any key to terminate...\n");
		//getch();
		return -1;
	}
	
	return 0;
}

void terminate(std::string deviceName) {
	portMap[deviceName]->closePort();
}
