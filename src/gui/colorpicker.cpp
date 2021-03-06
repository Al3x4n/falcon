#include "colorpicker.h"


Colorpicker::Colorpicker(Settings* p_glob_settings, Ui::MainWindow* p_ui):
  glob_settings(p_glob_settings),
  ui(p_ui)
{
  glob_settings->glob_args.gui_args.downlink_plot_color = QColor(40,110,255); //Blue
  glob_settings->glob_args.gui_args.uplink_plot_color   = QColor(255,110,40); //Orange

  dl_old = glob_settings->glob_args.gui_args.downlink_plot_color;
  ul_old = glob_settings->glob_args.gui_args.uplink_plot_color;


  downlink_palette.setColor(QPalette::Window, glob_settings->glob_args.gui_args.downlink_plot_color);
  uplink_palette.  setColor(QPalette::Window, glob_settings->glob_args.gui_args.uplink_plot_color);

  ui->label_dl_color->setAutoFillBackground(true);
  ui->label_dl_color->setPalette(downlink_palette);
  ui->label_ul_color->setAutoFillBackground(true);
  ui->label_ul_color->setPalette(uplink_palette);
  //ui->color_label->setText("What ever text");

  color_dialog = new QColorDialog(ui->waterfall_frame);
  color_dialog->setObjectName("CD");
  color_dialog->setWindowTitle("Color Dialog");
  color_dialog->setGeometry(0,0,100,100);

  connect(color_dialog,SIGNAL(currentColorChanged(const QColor)),SLOT(set_color(const QColor)));
  connect(color_dialog, SIGNAL(rejected()), SLOT(restore_color()));
  // ! Contrast adjustment
  color_range_slider = new RangeWidget(Qt::Horizontal,ui->waterfall_frame);
  color_range_slider->setObjectName(QStringLiteral("horizontalSlider"));
  // color_range_slider->setOrientation(Qt::Horizontal);
  color_range_slider->setRange(0,50000);
  color_range_slider->setFirstValue(0);
  color_range_slider->setSecondValue(50000);

  ui->contrast_adjustment_layout->replaceWidget(ui->contrast_adjustment_widget, color_range_slider);
}

void Colorpicker::set_color(const QColor &color){

  // Change color of display Label:
  if(downlink_color_active){
    glob_settings->glob_args.gui_args.downlink_plot_color = color;
    downlink_palette.setColor(QPalette::Window, glob_settings->glob_args.gui_args.downlink_plot_color);
    ui->label_dl_color->setPalette(downlink_palette);
  }else{
    glob_settings->glob_args.gui_args.uplink_plot_color = color;
    uplink_palette.setColor(QPalette::Window, glob_settings->glob_args.gui_args.uplink_plot_color);
    ui->label_ul_color->setPalette(uplink_palette);
  }

  emit color_change();
}

void Colorpicker::restore_color(){
  bool old_val = downlink_color_active;
  downlink_color_active = true;
  set_color(dl_old);
  downlink_color_active = false;
  set_color(ul_old);
  downlink_color_active = old_val;
}

void Colorpicker::on_pushButton_downlink_color_clicked()
{
  dl_old = glob_settings->glob_args.gui_args.downlink_plot_color;
  ul_old = glob_settings->glob_args.gui_args.uplink_plot_color;
  downlink_color_active = true;
  color_dialog->show();
}

void Colorpicker::on_pushButton_uplink_color_clicked()
{
  downlink_color_active = false;
  dl_old = glob_settings->glob_args.gui_args.downlink_plot_color;
  ul_old = glob_settings->glob_args.gui_args.uplink_plot_color;
  color_dialog->show();
}

RangeWidget* Colorpicker::get_color_range_slider(){
  return color_range_slider;
}

Colorpicker::~Colorpicker(){}
