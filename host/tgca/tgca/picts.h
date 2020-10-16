#ifndef PICTS_H
#define PICTS_H

/// \file      pict.h
///
/// \brief     Перечисление всех пиктограмм, используемых в файлах *.cpp программы tgca.
/// \details
///
/// \copyright ЗАО НТЦ "Модуль"
/// \author    Мисюрёва Е.В.
///
/// \date      17.12.2018
/// \version   0.1
///
/// История изменений:
///

#include <string>

/// кнопки на тесте
const std::string strPictTestButtonStart(":/pictogram/gtk-media-play-ltr_8717.png");
const std::string strPictTestButtonPause(":/pictogram/gtk-media-pause_2289.png");
const std::string strPictTestButtonStop(":/pictogram/gtk-media-stop_9402.png");

/// состояние теста
//const std::string strPictTestStatusRunning(":/pictogram/control_play_blue_7261.png");
const std::string strPictTestStatusInPause(":/pictogram/control_pause_blue_1763.png");
const std::string strPictTestStatusInterrupted(":/pictogram/searchCancelPressed.png");  // gtk-media-stop_9402.png  // /pictogram/Qt/x.png
const std::string strPictTestStatusNotStarted(":/pictogram/catch.png");    // catch.png    // gtk-media-stop_9402.png
const std::string strPictTestStatusFinishedOk(":/pictogram/ok.png");         // confirm_3843.png
const std::string strPictTestStatusFinishedFail(":/pictogram/button_cancel_6086.png");
const std::string strPictTestStatusFinishedErr(":/pictogram/agt_action_fail1_3562.png"); // (":/pictogram/close_5496.png");  // (":/pictogram/cancel_8315.png");

/// маркеры начала и конца запуска
const std::string strPictTestMarkerStart(":/pictogram/down.png");
const std::string strPictTestMarkerEnd(":/pictogram/bullet_delete_3633.png");

#endif // PICTS_H
