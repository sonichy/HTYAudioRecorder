#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(400,500));
    ui->pushButtonContinue->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonStop->hide();
    ui->sliderProgress->setVisible(false);
    move((QApplication::desktop()->width() - width())/2, (QApplication::desktop()->height() - height())/2);
    connect(ui->action_quit,SIGNAL(triggered(bool)),qApp,SLOT(quit()));

    directory = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first() + "/HTYAR";
    QDir *dir = new QDir(directory);
    if(!dir->exists()){
        dir->mkdir(directory);
    }

    model = new QFileSystemModel;
    model->setRootPath(directory);
    // 没有通过过滤器的文件，true为不可用，false为隐藏
    model->setNameFilterDisables(false);
    // 按时间倒序排列
    model->sort(3,Qt::DescendingOrder);
    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index(directory));
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(play(QModelIndex)));
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(viewContextMenu(QPoint)));

    player = new QMediaPlayer;
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChange(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));

    connect(ui->sliderProgress,SIGNAL(sliderReleased()),this,SLOT(setMPPosition()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRec_clicked()
{    
    ui->pushButtonRec->hide();
    ui->pushButtonPause->show();
    ui->pushButtonStop->show();
    ui->sliderProgress->setVisible(false);
    audioRecorder = new QAudioRecorder;
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/PCM");
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioRecorder->setEncodingSettings(audioSettings);
    audioRecorder->setContainerFormat("wav");
    QDateTime time = QDateTime::currentDateTime();
    QString filename = "录音" + time.toString("yyyyMMddhhmmss") + ".wav";
    QUrl url = QUrl::fromLocalFile(directory + "/" + filename);
    audioRecorder->setOutputLocation(url);
    audioRecorder->record();
    ui->statusBar->showMessage("录音开始 " + url.toString());
}

void MainWindow::on_pushButtonContinue_clicked()
{
    ui->pushButtonContinue->hide();
    ui->pushButtonPause->show();
    ui->pushButtonStop->show();
    audioRecorder->record();
    ui->statusBar->showMessage("录音继续");
}

void MainWindow::on_pushButtonPause_clicked()
{
    ui->pushButtonPause->hide();
    ui->pushButtonStop->show();
    ui->pushButtonContinue->show();
    audioRecorder->pause();
    ui->statusBar->showMessage("录音暂停");
}

void MainWindow::on_pushButtonStop_clicked()
{
    ui->pushButtonRec->show();
    ui->pushButtonContinue->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonStop->hide();
    audioRecorder->stop();
    ui->statusBar->showMessage("录音结束");
}

void MainWindow::on_action_directory_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(directory));
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰录音 1.0\n一款基于Qt的录音程序。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：sonichy.96.lt");
    QPixmap pixmap;
    pixmap.load(":/icon.ico");
    aboutMB.setIconPixmap(pixmap);
    aboutMB.exec();
}

void MainWindow::play(QModelIndex index)
{
    ui->sliderProgress->setVisible(true);
    ui->statusBar->showMessage("播放 " + directory + "/" + index.data().toString());
    player->setMedia(QUrl::fromLocalFile(directory + "/" + index.data().toString()));
    player->play();
}

void MainWindow::durationChange(qint64 d){
    ui->sliderProgress->setMaximum(d);
    //qDebug() << "player->duration()=" << player->duration() << d;
}

void MainWindow::positionChange(qint64 p){
    ui->sliderProgress->setValue(p);
}

void MainWindow::setMPPosition(){
    player->setPosition(ui->sliderProgress->value());
}

void MainWindow::viewContextMenu(const QPoint &position)
{
    QList<QAction *> actions;
    QAction *action_delete;
    QModelIndex index = ui->listView->indexAt(position);
    qDebug() << "index" << index;
    QString filepath = index.data(QFileSystemModel::FilePathRole).toString();
    //qDebug() << filepath;
    action_delete = new QAction(this);
    action_delete->setText("删除");
    actions.append(action_delete);
    if(!index.isValid()){
        action_delete->setVisible(false);
    }else{
        QAction *result_action = QMenu::exec(actions,ui->listView->mapToGlobal(position));
        if(result_action == action_delete){
            if(QFile::remove(filepath)){
                ui->statusBar->showMessage("已删除 " + filepath);
            }else{
                QMessageBox::critical(NULL, "错误", "无法删除文件 " + filepath);
            }
        }
    }
}
