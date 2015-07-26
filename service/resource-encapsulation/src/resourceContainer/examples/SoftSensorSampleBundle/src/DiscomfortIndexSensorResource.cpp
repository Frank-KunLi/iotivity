//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <algorithm>

#include "DiscomfortIndexSensorResource.h"

DiscomfortIndexSensorResource::DiscomfortIndexSensorResource()
{
}

DiscomfortIndexSensorResource::DiscomfortIndexSensorResource(vector<string> inputAttributes)
{
    m_pDiscomfortIndexSensor = new DiscomfortIndexSensor(inputAttributes);
    m_vecInputAttributes = inputAttributes;
    std::cout << "Init attributes" << endl;
    initAttributes();
    std::cout << "Init attributes finished" << endl;
}

DiscomfortIndexSensorResource::~DiscomfortIndexSensorResource()
{
    delete m_pDiscomfortIndexSensor;
}

void DiscomfortIndexSensorResource::initAttributes()
{

    BundleResource::setAttribute("temperature", "23");
    BundleResource::setAttribute("humidity", "40");
    BundleResource::setAttribute("discomfortIndex", "5");
}

ResourceAttributes &DiscomfortIndexSensorResource::getAttributes()
{
    return BundleResource::getAttributes();
}

ResourceAttributes::Value DiscomfortIndexSensorResource::getAttribute(const std::string &key)
{
    cout << "DiscomfortIndexSensorResource::getAttribute called for " << key << " called" << endl;
    return BundleResource::getAttribute(key);
}

void DiscomfortIndexSensorResource::setAttribute(std::string key, ResourceAttributes::Value &&value)
{
    cout << "DiscomfortIndexSensorResource::setAttribute setting " << key << " to " << value.toString()
         << std::endl;
    BundleResource::setAttribute(key, std::move(value));
}

void DiscomfortIndexSensorResource::setInputAttribute(SensorData input)
{
    vector<string>::iterator itor = std::find(m_vecInputAttributes.begin(), m_vecInputAttributes.end(),
                                    input.sensorName);
    std::vector<SensorData> inData;

    if (itor != m_vecInputAttributes.end())
    {
        m_mapStoredInputData[input.sensorName] = input;

        if (inputCount == m_mapStoredInputData.size())
        {
            for (map< string, SensorData >::iterator mapItor = m_mapStoredInputData.begin();
                 mapItor != m_mapStoredInputData.end(); mapItor++)
            {
                inData.push_back(mapItor->second);
            }

            m_pDiscomfortIndexSensor->runLogic(inData);
            m_outputs = m_pDiscomfortIndexSensor->m_output;

            for (unsigned int i = 0; i < m_outputs.data.size(); i++)
            {
                if (!m_outputs.data.at(i)["name"].compare("temperature"))
                    BundleResource::setAttribute("temperature",
                                                 ResourceAttributes::Value(m_outputs.data.at(i)["value"].c_str()));

                else if (!m_outputs.data.at(i)["name"].compare("humidity"))
                    BundleResource::setAttribute("humidity",
                                                 ResourceAttributes::Value(m_outputs.data.at(i)["value"].c_str()));

                else if (!m_outputs.data.at(i)["name"].compare("discomfortIndex"))
                    BundleResource::setAttribute("discomfortIndex",
                                                 ResourceAttributes::Value(m_outputs.data.at(i)["value"].c_str()));
            }
        }
    }
}