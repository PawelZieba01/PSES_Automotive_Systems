/**************************************************************************************************
 * Dio.h
 *
 * AUTOSAR Digital I/O (DIO) Module Header
 **************************************************************************************************/

#ifndef DIO_H
#define DIO_H

#include "Std_Types.h"

/* Typy danych */
typedef uint32 Dio_ChannelType;
typedef uint32 Dio_PortType;
typedef uint8  Dio_LevelType;
typedef uint32 Dio_PortLevelType;

/* Kanały DIO (przykładowe definicje) */
#define DioConf_DioChannel_MY_LED_CYCLIC 0U

/* Funkcje DIO */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

#endif /* DIO_H */