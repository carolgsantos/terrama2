var TcpManager = require("./../../core/TcpManager");
var DataManager = require("./../../core/DataManager");
var Utils = require('./../../core/Utils');

var _handleError = function(response, err) {
  console.log(err);
  Utils.handleRequestError(response, err, 400);
};

module.exports = function(app) {
  return {
    post: function(request, response) {
      var serviceId = request.body.serviceId;
      DataManager.getServiceInstance({id: serviceId}).then(function(serviceInstance) {
        TcpManager.startService(serviceInstance).then(function(code) {
          // sending update service with data
          TcpManager.updateService(serviceInstance).then(function(serviceStatus) {

            // getting all data providers
            var dataProvidersResult = DataManager.listDataProviders();
            var providers = [];
            dataProvidersResult.forEach(function(dataProvider) {
              providers.push(dataProvider.toObject())
            })

            // getting dataseries
            DataManager.listDataSeries().then(function(dataSeriesResult) {
              var series = [];
              dataSeriesResult.forEach(function(dataSeries) {
                series.push(dataSeries.toObject());
              })

              // getting collectors
              DataManager.listCollectors().then(function(collectorsResult) {
                var collectors = [];
                collectorsResult.forEach(function(collector) {
                  collectors.push(collector.toObject());
                })

                // getting analyses
                DataManager.listAnalyses().then(function(analysesResult) {
                  var analyses = [];
                  analysesResult.forEach(function(analysis) {
                    analyses.push(analysis.toObject());
                  });
                  
                  // sending everything
                  TcpManager.sendData({
                    "Analysis": analyses,
                    "DataSeries": series,
                    "DataProviders": providers,
                    "Collectors": collectors
                  })

                  // todo: check it
                  response.json({status: 200, online: code == 0 && serviceStatus.start_time});

                }).catch(function(err) {
                  _handleError(response, err)
                });
              }).catch(function(err) {
                _handleError(response, err)
              });
            }).catch(function(err) {
              _handleError(response, err)
            });
          }).catch(function(err) {
            _handleError(response, err);
          });
        }).catch(function(err) {
          _handleError(response, err);
        });
      }).catch(function(err) {
        _handleError(response, err);
      });
    },

    ping: function(request, response) {
      var serviceId = request.body.serviceId;
      DataManager.getServiceInstance({id: serviceId}).then(function(serviceInstance) {
        response.json({status: 200, online: false})
        // TcpManager.checkStatus(serviceInstance).then(function() {
        //   //todo: check it
        //   response.json({status: 200});
        // }).catch(_handleError);
      }).catch(function(err) {
        _handleError(response, err);
      });
    }
  }
}