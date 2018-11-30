/*
 * application.c
 *
 *  Created on: 02.10.2018
 *      Author: Michael Hoerner
 */

#include "DSP28x_Project.h"
#include "../main_c28.h"
#include "../include/application.h"


void Initialize_Application(DS_Data* data) {

data->appV.TestVariable1=42.0;
}

void assignGlobalDataToApplication(DS_Data* data) {

}

void assignApplicationDataToGlobalData(DS_Data* data) {

}


