/*
  Copyright (C) 2007 National Institute For Space Research (INPE) - Brazil.

  This file is part of TerraMA2 - a free and open source computational
  platform for analysis, monitoring, and alert of geo-environmental extremes.

  TerraMA2 is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  TerraMA2 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with TerraMA2. See LICENSE. If not, write to
  TerraMA2 Team at <terrama2-team@dpi.inpe.br>.
*/

/*!
  \file terrama2/services/collector/core/Collector.hpp

  \brief Model class for the collector configuration.

  \author Jano Simas
*/

#ifndef __TERRAMA2_SERVICES_COLLECTOR_CORE_SERVICE_HPP__
#define __TERRAMA2_SERVICES_COLLECTOR_CORE_SERVICE_HPP__

#include "../../../core/utility/Service.hpp"
#include "../../../core/Typedef.hpp"
#include "../../../core/Shared.hpp"
#include "Shared.hpp"
#include "Typedef.hpp"
#include "DataManager.hpp"
#include "CollectorLogger.hpp"

// STL
#include <memory>

namespace terrama2
{
  namespace services
  {
    namespace collector
    {
      namespace core
      {
        /*!
          \brief The %Collector Service provides thread and time management for Collector processes.

          This class is used to manage thread sync and timer listening to access
          process and store data based on Collector.

          The %Collector Service has a main thread that will check for new data to collect
          and a threadpool that will be allocated to actively collect and store the data.
        */
        class Service : public terrama2::core::Service
        {
            Q_OBJECT

          public:
            Service(std::weak_ptr<DataManager> dataManager);

            ~Service() = default;
            Service(const Service& other) = delete;
            Service(Service&& other) = default;
            Service& operator=(const Service& other) = delete;
            Service& operator=(Service&& other) = default;

          public slots:
            //! Slot to be called when a DataSetTimer times out.
            void addToQueue(CollectorId collectorId);
            /*!
              \brief Add a Collector to the service

              Check if this is the instance where the colelctor should run.
            */
            void addCollector(CollectorPtr);

            /*!
              \brief Updates the Collector.

              calls addCollector()
            */
            void updateCollector(CollectorPtr collector);
            /*!
              \brief Removes the Collector.

              Rennuning processes will continue until finished.
            */
            void removeCollector(CollectorId collectorId);

            //doc in base class
            virtual void updateNumberOfThreads(int) override;

            //doc in base class
            virtual void updateLoggerConnectionInfo(const std::map<std::string, std::string>& connInfo) override;

          protected:
            // comments on base class
            virtual bool hasDataOnQueue() noexcept override;
            // comments on base class
            virtual bool processNextData() override;

            //*! Create a process task and add to taskQueue_
            virtual void prepareTask(CollectorId collectorId);
            /*!
              \brief Callback method to collect and store data.
            */
            static void collect(CollectorId collectorId, std::shared_ptr< CollectorLogger > logger, std::weak_ptr<DataManager> weakDataManager);

            //! Connects signals from DataManager
            void connectDataManager();

            std::weak_ptr<DataManager> dataManager_; //!< Weak pointer to the DataManager

            std::map<CollectorId, terrama2::core::TimerPtr> timers_;//!< List of running Collector timers
            std::deque<CollectorId> collectorQueue_;//!< Collector queue
            std::shared_ptr< CollectorLogger > logger_;//!< process logger
        };

      } // end namespace core
    }   // end namespace collector
  }     // end namespace services
} // end namespace terrama2

#endif //__TERRAMA2_SERVICES_COLLECTOR_CORE_SERVICE_HPP__
