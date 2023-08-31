/*
 * address.h
 *
 *  Created on: 3 Mar 2023
 *      Author: jacques
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_

//padded to 16 bytes on local machine for testing
struct Address
{
	void* blockPtr; //8 bytes
	short offset;//2 bytes
};

#endif /* ADDRESS_H_ */
