#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <datapoint.h>
#include <database.h>
#include <regulator.h>
#include <ammeter.h>
#include <QTimer>
#include <QList>
#include <QTime>

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = 0);
    ~DataProcessor();
    datatype getLastPower();//时间单位 s，获取单位时间内的平均功率
    DataPoint getLastData();//读取最新数据
    datatype getAveragePower(int timeLength);//second  设定时间内的总做功 totalpower 也在这个函数中计算
    datatype getMinPower(int timeLength);//second
    void setTimeInterval(int interval,int timettype);//设置不同时间间隔
    void setProportion(float proportion);//设置调节限位比例
    DataPoint getMinPowerDataPoint(int timeLength);//搜索最小功率的数据点
    void dataSlicer(QDateTime begin,QDateTime end,QList<DataPoint> &datapoints, int dataamount);
//    float getSavingRate();//结算节电率
    void rewritePowerMessage(datatype &ap, datatype &tp, datatype &up, int timeLength);//average total up
    bool isAmmeterError();
    bool isRegulatorError();
signals:
    void ammeterError();//电表故障
    void dataBaseError();//数据库故障
    void regulatorError();//下位机动作故障
    void actionError();//不合理操作
    void regulatorFinish();//下位机操作完成
    void newRealTimeData();//获得新的数据，通知UI绘图
    void monitorBegun();//监控开始
    void monitorFinish();//监控结束
    void sendTestResult(datatype powerBefore,datatype powerAfter,float ratio);//将节电测试的结果发往testWindow
    void connectSuccess();//连接电表和下位机成功
public slots:
    void regulatorAction();//调节下位机操作
    void testAction();//节电测试
    void closeMonitor();
    void openMonitor();
    void regulatorConnectError();
    void dataStartWork();//开始计时工作
#ifndef TESTMODE
    void connectammeter(QString ammetername);//连接电表
#endif
    void connectregulator(QString regulatorname);//连接下位机

private slots:
    void getData();//UI通过dataprocessor获得数据
    void ammeterGetDataError();//电表读数错误
    void saveData();//将数据存储到database
    void monitorAction();//每隔一段时间进行监控操作
    void testStart();//测试开始
    void testCount();//测试结算

private:
    Database *database;//数据库实例
    Regulator *regulator;//下位机动作实例
    Ammeter *ammeter;//电表实例
    QTimer *saveDataTimer;//默认每10秒发动一次
    QTimer *monitorTimer;//默认每10分钟发动一次 
    QList<DataPoint> realTimeDataBuffer;//1point/1s,length=3600
    datatype getMinPowerVoltage(int timeLength);//找到最低功率点的电压值
    datatype AveragePower;//当前监控点的平均功率
    datatype TotalPower;//当前时间间隔内的总功率
    datatype MinPower;//瞬时功率最小值
    float ProporitonLimit;//限位比例
    datatype PowerBeforeTest;
    datatype PowerAfterTest;
    float SavingRate;//节电率
    int   GetDataTimeInterval;//采样数据间隔 单位：Second
    int   SaveDataTimeInterval;//存储数据间隔  单位：Second
    int   MonitorTimeInterval;//监控数据间隔 单位：Second
    DataPoint MinPowerDataPoint;//最小功率数据点
    bool AmmeterError;
    bool RegulatorError;

};

#endif // DATAPROCESSOR_H
