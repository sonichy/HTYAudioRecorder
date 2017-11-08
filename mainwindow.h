#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QAudioRecorder>
#include <QApplication>
#include <QDesktopWidget>
#include <QUrl>
#include <QStandardPaths>
#include <QFileSystemModel>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAudioRecorder *audioRecorder;
    QString directory;
    QFileSystemModel *model;
    QMediaPlayer *player;

private slots:
    void on_action_directory_triggered();
    void on_action_about_triggered();
    void on_pushButtonRec_clicked();
    void on_pushButtonContinue_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();
    void play(QModelIndex);
    void durationChange(qint64 d);
    void positionChange(qint64 p);
    void setMPPosition();
    void viewContextMenu(const QPoint &position);
};

#endif // MAINWINDOW_H
