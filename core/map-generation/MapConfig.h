// MapGeneratorConfig.h
#pragma once

namespace map_config {

    // пороги высоты: низина / холмы / горы
    inline constexpr double ALTITUDE_LOW_THRESHOLD  = 0.33;
    inline constexpr double ALTITUDE_MID_THRESHOLD  = 0.75;

    // порог температуры: холодно / тепло
    inline constexpr double TEMPERATURE_WARM_THRESHOLD = 0.50;

    // порог влажности: сухо / влажно
    inline constexpr double MOISTURE_WET_THRESHOLD = 0.50;

    // отдельный порог для снежных скал (холодно + очень влажно)
    inline constexpr double MOISTURE_SNOWY_ROCKS_THRESHOLD = 0.75;

    // значения после упрощения: типа "низкое/среднее/высокое"
    inline constexpr double SIMPLIFIED_LOW_VALUE  = 0.10;
    inline constexpr double SIMPLIFIED_MID_VALUE  = 0.50;
    inline constexpr double SIMPLIFIED_HIGH_VALUE = 0.90;

    // пороги, когда упрощаем температуру/влажность в 0.1 или 0.9
    inline constexpr double SIMPLIFIED_TEMPERATURE_THRESHOLD = 0.50;
    inline constexpr double SIMPLIFIED_MOISTURE_THRESHOLD    = 0.50;

    // пороги для упрощения высоты в 0.1 / 0.5 / 0.9
    inline constexpr double SIMPLIFIED_ALTITUDE_LOW_THRESHOLD = 0.33;
    inline constexpr double SIMPLIFIED_ALTITUDE_MID_THRESHOLD = 0.75;


    // границы размеров, от которых зависит масштабирование
    inline constexpr double SIZE_THRESHOLD_SMALL  = 10.0;
    inline constexpr double SIZE_THRESHOLD_MEDIUM = 30.0;

    // дефолтный scale для перлина
    inline constexpr double DEFAULT_SCALE_SMALL  = 60.0;
    inline constexpr double DEFAULT_SCALE_MEDIUM = 80.0;
    inline constexpr double DEFAULT_SCALE_LARGE  = 140.0;

    // во сколько раз увеличиваем пиксельную карту (до усреднения в grid)
    inline constexpr int PIXEL_UPSCALE_SMALL  = 4;
    inline constexpr int PIXEL_UPSCALE_MEDIUM = 3;
    inline constexpr int PIXEL_UPSCALE_LARGE  = 2;

    // размер блока пикселей, которые усредняются в одну клетку grid (3x3)
    inline constexpr int GRID_BLOCK_SIZE = 3;


    // смещения, чтобы слои шума не совпадали
    inline constexpr int OFFSET_TEMPERATURE = 0;
    inline constexpr int OFFSET_ALTITUDE    = 5500;
    inline constexpr int OFFSET_MOISTURE    = 12000;

    // параметры шума: (octaves, persistence)
    inline constexpr int    TEMP_OCTAVES = 3;
    inline constexpr double TEMP_PERSISTENCE = 0.40;

    inline constexpr int    ALT_OCTAVES = 4;
    inline constexpr double ALT_PERSISTENCE = 0.50;

    inline constexpr int    MOIST_OCTAVES = 2;
    inline constexpr double MOIST_PERSISTENCE = 0.50;

    // если вдруг весь слой одинаковый
    inline constexpr double NORMALIZE_EPS = 0.001;

    // доп множители к scale, чтобы по осям был другой "рисунок"
    inline constexpr double SIMPLIFIED_SCALE_X_MULT = 0.70;
    inline constexpr double SIMPLIFIED_SCALE_Y_MULT = 0.75;

    inline constexpr int    SIMPLIFIED_OCTAVES = 4;
    inline constexpr double SIMPLIFIED_PERSISTENCE = 0.50;

    // пороги выбора "типа" клетки в simplified карте
    inline constexpr double SIMPLIFIED_CLASS_HIGH_THRESHOLD = 0.65;
    inline constexpr double SIMPLIFIED_CLASS_MID_THRESHOLD  = 0.40;

}

