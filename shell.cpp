#include "shell.h"
#include "ui_shell.h"

#include "exec_cmd.h"
#include "time_counter.h"

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>
#include <QDir>
#include <QMessageBox>
extern QString project_dir;
static QString base_xml_dir = "/mnt/share/3.project/iceberg/deploy.xml.d/";

shell::shell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shell)
{
    ui->setupUi(this);
    update_whole_xml();
    ui->iplist->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
}

shell::~shell()
{
    delete ui;
}

void shell::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}

void shell::update_whole_xml()
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

void shell::on_addip_clicked()
{
    add_ip_tolist(ui->ip->text());
}

void shell::add_ip_tolist(const QString& ip)
{
    if(ip == "")
    {
        return;
    }
    auto res_list = ui->iplist->findItems(ip,Qt::MatchExactly);
    if(res_list.size() > 0)
    {
        return;
    }
    ui->iplist->addItem(ip);
}

QString shell::deply_xml_path()
{
   auto xml = base_xml_dir + ui->xml->currentText()+".xml";
   return xml;
}

void shell::on_add_clicked()
{
    if(ui->xml->currentText() == "")
    {
        return;
    }
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_node.sh " + project_dir +
            " " + deply_xml_path();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj ips;
    try
    {
        ips.loads(output.c_str());
        for (auto node_ip : ips.array_val)
        {
            add_ip_tolist(QString(node_ip.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("update_node failed");
    }
}

void shell::on_clear_clicked()
{
    ui->iplist->clear();
}


std::string local_tmp_shell = "remote_exec_tmp.sh";
void shell::on_exe_clicked()
{
    if(ui->iplist->count() == 0)
    {
        alert("IP List is empty");
        return;
    }

    if(ui->logs->toPlainText()=="")
    {
        alert("empty shell is denied");
        return;
    }
    gen_shell();
    json_obj after_cmds;
    auto basedir = QDir::currentPath();
    auto cmd = "echo '---------------start---------------------'";
    for (int i = 0; i < ui->iplist->count(); i++)
    {
        QString ip = ui->iplist->item(i)->text();
        QString after_cmd  = basedir +"/../release_tool/copy_file.sh " + project_dir +
                " " + ip + " /root " + local_tmp_shell.c_str()
                +  " " + basedir + "/" + local_tmp_shell.c_str();
        after_cmds.append(json_obj(after_cmd.toStdString()));
        after_cmd = "ssh root@" + ip + " \"bash /root/" + local_tmp_shell.c_str() +"\"";
        after_cmds.append(json_obj(after_cmd.toStdString()));
    }
    exec_cmd * dlg = new exec_cmd("execute logs",cmd,after_cmds);
    dlg->exec();
    delete dlg;
}

void shell::on_remove_clicked()
{
    auto items = ui->iplist->selectedItems();

    for (auto item : items)
    {
        int r = ui->iplist->row(item);
        ui->iplist->takeItem(r);
    }

}


void shell::exec_on_ip(const QString &ip)
{
    json_obj after_cmds;
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/copy_file.sh " + project_dir +
            " " + ip + " /root " + local_tmp_shell.c_str()
            +  " " + basedir + "/" + local_tmp_shell.c_str();
    QString after_cmd = "ssh root@" + ip + " \"bash /root/" + local_tmp_shell.c_str() +"\"";
    after_cmds.append(json_obj(after_cmd.toStdString()));
    exec_cmd * dlg = new exec_cmd(ip,cmd,after_cmds);
    dlg->show();
}

void shell::gen_shell()
{
    QString str = ui->bash->text() + "\n";
    str +=  ui->logs->toPlainText();
    int err = write_str2file(local_tmp_shell.c_str(), str.toStdString().c_str());
    if(err)
    {
        alert("write_str2file failed");
    }

}
