/*
 * rotary.h
 *
 *  Created on: Oct 11, 2016
 *      Author: kent
 */

#ifndef ROTARY_H_
#define ROTARY_H_

void initADC();

uint32_t next();
uint32_t moisture();
void initTimer();
void sendRed();
void sendBlue();
void sendGreen();
void output();
int humidity();
void temp();
int yo();
int yo1();

#endif /* ROTARY_H_ */
