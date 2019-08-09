#include "log_collect.h"
#include "ui_log_collect.h"

#include <QDir>
#include <QMessageBox>
extern QString project_dir;
static QString base_xml_dir = "/mnt/share/3.project/iceberg/deploy.xml.d/";

log_collect::log_collect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log_collect)
{
    ui->setupUi(this);
    add_mainversions();
    update_service_name();
    ui->whole->setChecked(true);
    ui->service->setEnabled(false);
    update_view_after_radio_select();
}

void log_collect::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}

log_collect::~log_collect()
{
    delete ui;
}

void log_collect::on_fixname_clicked(bool checked)
{
    if(checked)
    {
        ui->bugnum->setEnabled(true);
        ui->main_version->setEnabled(false);
    }
    else
    {
        ui->main_version->setEnabled(true);
        ui->bugnum->setEnabled(false);
        ui->bugnum->setText("");
    }
}

void log_collect::update_service_name()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_services.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj all_services;
    try
    {
        all_services.loads(output.c_str());
        for (auto service : all_services.array_val)
        {
            ui->service->addItem(QString(service.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no tag now");
    }
}

void log_collect::add_mainversions()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/tags.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    try
    {
        version_map.loads(output.c_str());
        for ( auto tag = version_map.dic_val.rbegin();
              tag != version_map.dic_val.rend(); ++tag)
        {
            ui->main_version->addItem(QString(tag->first.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no tag now");
    }
}


void log_collect::update_view_after_radio_select()
{
    if(ui->single->isChecked())
    {
        ui->service->setEnabled(true);
        update_single_xml();
    }
    else
    {
        ui->service->setEnabled(false);
        update_whole_xml();
    }
}

void log_collect::update_whole_xml()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_whole_xml.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
        return ;
    }
    json_obj all_xmls;
    try
    {
        ui->xml->clear();
        all_xmls.loads(output.c_str());
        for ( auto xml = all_xmls.array_val.begin();
              xml != all_xmls.array_val.end(); ++xml)
        {
            ui->xml->addItem(QString(xml->s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no xml now");
    }
}

void log_collect::update_single_xml()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_service_xml.sh " + project_dir + " " + ui->service->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
        return ;
    }
    json_obj all_xmls;
    try
    {
        ui->xml->clear();
        all_xmls.loads(output.c_str());
        for ( auto xml = all_xmls.array_val.rbegin();
              xml != all_xmls.array_val.rend(); ++xml)
        {
            ui->xml->addItem(QString(xml->s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no xml now");
    }
}

void log_collect::on_single_clicked()
{
    update_view_after_radio_select();
}

void log_collect::on_whole_clicked()
{
    update_view_after_radio_select();
}

QString log_collect::deply_xml_path()
{
    QString xml;
    if(ui->single->isChecked())
    {
        xml = ui->service->currentText() + "/"+ui->xml->currentText();
    }
    else
    {
        xml = ui->xml->currentText();
    }
    return xml;
}

void log_collect::on_start_clicked()
{
    QString name="";
    if(ui->fixname->isChecked())
    {
        name = ui->bugnum->text();
    }
    else
    {
        name = ui->main_version->currentText();
    }

    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/collect.sh " + project_dir + " " + deply_xml_path() + " " + name;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("deploy wrong:" + QString(output.c_str()));
    }
    else
    {
        start_update_data();
    }
}


QString log_collect::md5_log()
{
    auto cmd = "md5sum " + project_dir + "/collect.log";
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        return QString("");
    }
    else
    {
        return QString(output.c_str());
    }
}

void log_collect::update_logs()
{
    QString cmd = "cat " + project_dir + "/collect.log" ;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
    }
    else
    {
        ui->logs->setText(QString(output.c_str()));
        QTextCursor cursor = ui->logs->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->logs->setTextCursor(cursor);
        update_log_color();
    }
}

void log_collect::update_log_color()
{
    ui->logs->setTextColor(Qt::white);
    update_color_key_color(ui->logs,QString("start"),Qt::green);
    update_color_key_color(ui->logs,QString("end"),Qt::green);
    update_color_key_color(ui->logs,QString("ok"),Qt::green);
    update_color_key_color(ui->logs,QString("failed"),Qt::red);
    update_color_key_color(ui->logs,QString("not"),Qt::yellow);
    update_color_key_color(ui->logs,QString("install"),Qt::yellow);
    update_color_key_color(ui->logs,QString("172.30.41.8:5000"),Qt::cyan);
}

void log_collect::update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush)
{
    QString searchString = key;
    QTextDocument *document = item->document();

    if (searchString.isEmpty()) {
        return;
    }
    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(brush);

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(searchString, highlightCursor,
                                         QTextDocument::FindWholeWords);

        if (!highlightCursor.isNull()) {
            highlightCursor.movePosition(QTextCursor::WordRight,
                                         QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }
    cursor.endEditBlock();
}


void log_collect::timerEvent(QTimerEvent *event)
{
    QString cur_md5 = md5_log();
    if(last_md5 == cur_md5)
    {
        same_cnt++;
        if(same_cnt == 120)
        {
            killTimer(id);
            ui->start->setEnabled(true);
        }
    }
    else
    {
        same_cnt = 0;
        update_logs();
        last_md5 = cur_md5;
    }
}

void log_collect::start_update_data()
{
    ui->start->setEnabled(false);
    id = startTimer(1000);
}
