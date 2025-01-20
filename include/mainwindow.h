#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
 * @file      mainwindow.h
 * @brief     Header of
 * @date      Thu Jul  7 09:48:51 2022
 * @author    aightech
 * @copyright BSD-3-Clause
 *
 * This module
 */

#include "qboxlayout.h"
#include "qcustomplot.h"
#include <QMainWindow>
#include <QSettings>
#include <QTreeWidgetItem>
#include <clvHdMaster.hpp>
#include <queue>

#include <QTimer>
#include <chrono>
#include <iomanip> // std::setprecision
#include <iostream>
#include <lsl_cpp.h>
#include <numeric>
#include <pthread.h>
#include <utility>
#include "fltlib.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

using clk = std::chrono::system_clock;
using sec = std::chrono::duration<double>;

/**
 * @brief The MainWindow class
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief addBoard Add a new board to the list of boards.
     *
     * @param id
     */
    void
    addBoard(int id);

    /**
     * @brief streaming_loop Loop that continuously reads data from the board and send it to the LSL stream.
     *
     * @param obj The MainWindow object.
     * @return void*
     */
    static void *
    streaming_loop(void *obj);

    /**
     * @brief push_lsl Push data to the LSL stream.
     *
     */
    void
    push_lsl();

    private slots:
    void
    connection();
    void
    update_indiv_param();
    void
    upload();
    void
    start_streaming();
    void
    stop_streaming();
    void
    debug();
    void
    filters();
    void
    write_reg();
    void
    read_reg();
    void
    data_ploting();

    void update_filter_param();

    private:
    Ui::MainWindow *ui;
    ClvHd::Master m_master;
    std::vector<QTreeWidgetItem *> m_boardItem;
    int m_nb_ch = 0;
    int m_nb_board;
    std::vector<std::pair<int, uint8_t>> m_active_channel;
    std::vector<std::pair<int, int>> m_active_channel_ind;
    bool m_is_streaming = false;
    bool m_active = true;
    pthread_t m_thread;
    lsl::stream_outlet *m_lsl_outlet;
    std::vector<float> m_lsl_sample;
    double m_t = 0;

    QHBoxLayout *m_debug_bar;
    bool m_debug_mode = false;
    bool m_filter_mode = false;

    int m_nb_bytes = 2;

    QSettings *m_settings;

    std::chrono::time_point<std::chrono::system_clock> m_time;
    std::chrono::time_point<std::chrono::system_clock> m_start_time;
    sec m_duration;

    QCustomPlot *m_plot=nullptr;
    double m_prev_val = 0;


    int m_plot_t_max = 5;
    //queue of the raw data
    std::vector<std::queue<float>> m_raw_data;
    //size of the queue
    int m_data_queue_size = 100;

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    QTimer m_plotTimer;

    //filters
    std::vector<std::vector<Filter*>> m_filters;
    double m_sampling_rate = 2000;
    double m_fs=1000;
    std::vector<QCheckBox*> m_filter_checkboxes;
};
#endif // MAINWINDOW_H
