#include "OsuBeatmap.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "../../Utils.h"
#include "OsuTimingPoint.h"
#include "OsuHitObject.h"

//This it to keep track in which section of the file we are while parsing it
enum FileFormatSection {
    None,
    General,
    Editor,
    Metadata,
    Difficulty,
    Events,
    TimingPoints,
    Colours,
    HitObjects,
};


static void HandleGeneralLine(OsuBeatmap *beatmap, char *line) {
    if (strncmp(line, "AudioFilename: ", 15) == 0) {
        strncpy(beatmap->m_audio_filename, line+15, 32);
        printf("AudioFilename: %s\n", beatmap->m_audio_filename);
    }
    else if (strncmp(line, "AudioLeadIn: ", 13) == 0) {
        beatmap->m_audio_lead_in = atoi(line+13);
        printf("AudioLeadIn: %d\n", beatmap->m_audio_lead_in);
    }
    else if (strncmp(line, "PreviewTime: ", 13) == 0) {
        beatmap->m_preview_time = atoi(line+13);
        printf("PreviewTime: %d\n", beatmap->m_preview_time);
    }
    else if (strncmp(line, "SampleSet: ", 11) == 0) {
        if (strncmp(line+11, "Normal", 6) == 0) beatmap->m_sample_set = 0;
        else if (strncmp(line+11, "Soft", 4) == 0) beatmap->m_sample_set = 1;
        else if (strncmp(line+11, "Drum", 4) == 0) beatmap->m_sample_set = 2;
        printf("SampleSet: %d\n", beatmap->m_sample_set);
    }
    else if (strncmp(line, "Mode: ", 6) == 0) {
        beatmap->m_mode = atoi(line+6);
        printf("Mode: %d\n", beatmap->m_mode);
    }
    else {
        printf("Unknown General field '%s'\n", line);
    }
}


static void HandleMetadataLine(OsuBeatmap *beatmap, char *line) {
    if (strncmp(line, "Title: ", 7) == 0) {
        strncpy(beatmap->m_title, line+7, 32);
        printf("Title: %s\n", beatmap->m_title);
    }
    else if (strncmp(line, "TitleUnicode: ", 14) == 0) {
        strncpy(beatmap->m_title_unicode, line+14, 32);
        printf("TitleUnicode: %s\n", beatmap->m_title_unicode);
    }
    else if (strncmp(line, "Artist: ", 8) == 0) {
        strncpy(beatmap->m_artist, line+8, 32);
        printf("Artist: %s\n", beatmap->m_artist);
    }
    else if (strncmp(line, "ArtistUnicode: ", 15) == 0) {
        strncpy(beatmap->m_artist_unicode, line+15, 32);
        printf("ArtistUnicode: %s\n", beatmap->m_artist_unicode);
    }
    else if (strncmp(line, "Creator: ", 9) == 0) {
        strncpy(beatmap->m_creator, line+9, 32);
        printf("Creator: %s\n", beatmap->m_creator);
    }
    else if (strncmp(line, "Version: ", 9) == 0) {
        strncpy(beatmap->m_version, line+9, 32);
        printf("Version: %s\n", beatmap->m_version);
    }
    else {
        printf("Unknown Metadata field '%s'\n", line);
    }
}


static void HandleDifficultyLine(OsuBeatmap *beatmap, char *line) {
    if (strncmp(line, "HPDrainRate: ", 13) == 0) {
        beatmap->m_hp_drain = atof(line+13);
        printf("HPDrainRate: %f\n", beatmap->m_hp_drain);
    }
    else if (strncmp(line, "CircleSize: ", 12) == 0) {
        beatmap->m_circle_size = atof(line+12);
        printf("CircleSize: %f\n", beatmap->m_circle_size);
    }
    else if (strncmp(line, "OverallDifficulty: ", 19) == 0) {
        beatmap->m_overall_difficulty = atof(line+19);
        printf("OverallDifficulty: %f\n", beatmap->m_overall_difficulty);
    }
    else if (strncmp(line, "ApproachRate: ", 14) == 0) {
        beatmap->m_approach_rate = atof(line+14);
        printf("ApproachRate: %f\n", beatmap->m_approach_rate);
    }
    else if (strncmp(line, "SliderMultiplier: ", 18) == 0) {
        beatmap->m_slider_multiplier = atof(line+18);
        printf("SliderMultiplier: %f\n", beatmap->m_slider_multiplier);
    }
    else if (strncmp(line, "SliderTickRate: ", 16) == 0) {
        beatmap->m_slider_tick_rate = atof(line+16);
        printf("SliderTickRate: %f\n", beatmap->m_slider_tick_rate);
    }
    else {
        printf("Unknown Difficulty field '%s'\n", line);
    }
}


//TODO: currently this is only used to get the background, it doesn't do anything else
void HandleEventLine(OsuBeatmap *beatmap, char *line) {
    //Event syntax: eventType,startTime,eventParams
    size_t line_length = strlen(line);
    size_t current_index = 0;
    int event_type = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int start_time = ParseInt(line, &current_index);
    ++current_index; //skip comma

    if (event_type != 0) return; //replace this with a switch case whenever we actually decide to support multiple event types ig (type 0 is background)

    //Background syntax: 0,0,filename,xOffset,yOffset   "Double quotes are usually included surrounding the filename, but they are not required." wth??
    char *file_name = ParseStrOsu(line, &current_index);
    if (file_name == nullptr) return;
    ++current_index; //skip comma

    //"If the offset is 0,0, writing it is optional" ?????? wat
    int x_offset, y_offset;
    if (current_index >= line_length) {
        x_offset = 0;
        y_offset = 0;
    }
    else {
        int x_offset = ParseInt(line, &current_index);
        ++current_index; //skip comma
        int y_offset = ParseInt(line, &current_index);
    }

    //TODO : handle offset (previously there was x_offset == 0 && y_offset == 0 in the condition)
    if (start_time == 0) beatmap->SetBackground(file_name); //this makes a copy of the string
    free(file_name);
}


static void HandleTimingPointLine(OsuBeatmap *beatmap, char *line) {
    //Timing point syntax: time,beatLength,meter,sampleSet,sampleIndex,volume,uninherited,effects
    size_t current_index = 0;
    int time = ParseInt(line, &current_index);
    ++current_index; //skip comma
    float beat_length = ParseFloat(line, &current_index);
    ++current_index; //skip comma
    int meter = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int sample_set = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int sample_index = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int volume = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int uninherited = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int effects = ParseInt(line, &current_index);

    if (beatmap->m_timing_point_parsed < beatmap->m_timing_point_count) {
        beatmap->m_timing_points[beatmap->m_timing_point_parsed] = OsuTimingPoint {
            time, beat_length, meter, sample_set, sample_index, volume, uninherited == 1, effects
        };
        //printf("%d,%f,%d,%d,%d,%d,%d,%d\n", time, beat_length, meter, sample_set, sample_index, volume, uninherited, effects);
        ++beatmap->m_timing_point_parsed;
    }
    else {
        printf("%d,%f,%d,%d,%d,%d,%d,%d\n", time, beat_length, meter, sample_set, sample_index, volume, uninherited, effects);
        printf("Reached end of timing points, ignoring\n");
    }
}


static void HandleHitObjectLine(OsuBeatmap *beatmap, char *line) {
    //Hit object syntax: x,y,time,type,hitSound,[objectParams,]hitSample
    size_t current_index = 0;
    int x = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int y = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int time = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int type = ParseInt(line, &current_index);
    ++current_index; //skip comma
    int hit_sound = ParseInt(line, &current_index);
    ++current_index; //skip comma

    //printf("%d,%d,%d,%d,%d,", x, y, time, type, hit_sound);

    if ((type&OsuHitObjectType::HitCircle) != 0) {
        beatmap->m_hit_objects[beatmap->m_hit_object_parsed] = OsuHitObject(x, y, time, type, hit_sound);

        //TODO: parse samples

        ++beatmap->m_hit_object_parsed;
        //printf("\n");
    }
    else if ((type&OsuHitObjectType::Slider) != 0) {
        //Slider syntax: x,y,time,type,hitSound,  curveType|curvePoints,slides,length,[edgeSounds,edgeSets,]  hitSample
        beatmap->m_hit_objects[beatmap->m_hit_object_parsed] = OsuHitObject(x, y, time, type, hit_sound);
        auto *slider_params = new OsuSliderHitObjectParams;
        beatmap->m_hit_objects[beatmap->m_hit_object_parsed].object_params = slider_params;

        //parse curve stuff
        slider_params->curve_type = line[current_index];
        ++current_index; //skip curve type
        size_t curve_point_count = CountCharUntil(line, '|', current_index, ',');
        if (curve_point_count != 0) {
            slider_params->curve_points = (CurvePoint *) malloc(sizeof(CurvePoint) * curve_point_count);
            slider_params->curve_point_count = curve_point_count;
        }
        ++current_index; //skip first pipe (or comma if there are no curvePoints)
        //printf("%c|", slider_params->curve_type);

        //parse curvePoints
        for (size_t i = 0; i < curve_point_count; ++i) {
            float curve_x = ParseInt(line, &current_index); //Use ParseFloat if stuff break
            ++current_index; //skip collon
            float curve_y = ParseInt(line, &current_index); //use ParseFloat
            ++current_index; //skip pipe (or last comma
            slider_params->curve_points[i] = {curve_x, curve_y};

            //if (i == curve_point_count-1) printf("%f:%f,", curve_x, curve_y);
            //else printf("%f:%f|", curve_x, curve_y);
        }

        slider_params->slides = ParseInt(line, &current_index);
        ++current_index; //skip comma
        slider_params->length = ParseFloat(line, &current_index);
        if (slider_params->length < 0) slider_params->length = 0; //info about negative slider length in taiko here https://www.reddit.com/r/osugame/comments/1jf8obl/bad_apple_but_it_uses_taiko_sliders_only_made_by/ (let's just set it to 0 and see if it works for now)
        ++current_index; //skip comma
        //printf("%d,%f,", slider_params->slides, slider_params->length); //slide,length,

        if (beatmap->m_mode != 1) { //Non taiko modes have more stuff here (edgeSounds,edgeSets)
            //TODO : parse curve stuff for standard (edgeSounds and edgeSets)
        }

        //TODO : parse samples

        ++beatmap->m_hit_object_parsed;
        //printf("\n");
    }
    else if ((type&OsuHitObjectType::Spinner) != 0) {
        //x,y,time,type,hitSound,endTime,hitSample
        beatmap->m_hit_objects[beatmap->m_hit_object_parsed] = OsuHitObject(x, y, time, type, hit_sound);

        int end_time = ParseInt(line, &current_index);
        ++current_index; //skip comma
        //printf("%d,", end_time);

        auto *spinner_params = new OsuSpinnerHitObjectParams(end_time);
        beatmap->m_hit_objects[beatmap->m_hit_object_parsed].object_params = spinner_params;

        //TODO : parse samples

        ++beatmap->m_hit_object_parsed;
        //printf("\n");
    }
    else {
        printf(" - Unknown HitObject type %d\n", type);
    }
}


//This will count the amount of non-empty line there is between the current line and the next line starting with '[' (or end of file)
static size_t CountElementsInSection(FILE *in_file) {
    size_t current_location = ftell(in_file);
    size_t hit_object_count = 0;

    char line[1024] = {0};
    while (fgets(line, 1024, in_file) != nullptr) {
        if (line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')) continue;
        if (line[0] == '/' && line[1] == '/') continue;
        if (line[0] == '[') break;

        ++hit_object_count;
    }

    fseek(in_file, current_location, SEEK_SET);
    return hit_object_count;
}

OsuBeatmap *OsuBeatmap::load(const char *path) {
    if (path == nullptr) return nullptr;

    FILE *in_file = fopen(path, "r");
    if (in_file == nullptr) return nullptr;

    OsuBeatmap *beatmap = new OsuBeatmap;
    FileFormatSection current_section = None;

    char line[1024] = {0};
    while (fgets(line, 1024, in_file) != nullptr) {
        if (line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')) continue;
        if (line[0] == '/' && line[1] == '/') continue;

        RemoveNewline(line);

        if (line[0] == '[') {
            if (strcmp(line, "[General]") == 0) current_section = General;
            else if (strcmp(line, "[Editor]") == 0) current_section = Editor;
            else if (strcmp(line, "[Metadata]") == 0) current_section = Metadata;
            else if (strcmp(line, "[Difficulty]") == 0) current_section = Difficulty;
            else if (strcmp(line, "[Events]") == 0) current_section = Events;
            else if (strcmp(line, "[TimingPoints]") == 0) {
                //TODO : handle the case where we already encountered a TimingPoints section?
                current_section = TimingPoints;
                beatmap->m_timing_point_count = CountElementsInSection(in_file);
                printf("Counted %zu timing points\n", beatmap->m_timing_point_count);
                beatmap->m_timing_points = (OsuTimingPoint *) malloc(sizeof(OsuTimingPoint) * beatmap->m_timing_point_count);
            }
            else if (strcmp(line, "[Colours]") == 0) current_section = Colours;
            else if (strcmp(line, "[HitObjects]") == 0) {
                //TODO : handle the case where we already encountered an HitObjects section?
                current_section = HitObjects;
                beatmap->m_hit_object_count = CountElementsInSection(in_file);
                printf("Counted %zu hit objects\n", beatmap->m_hit_object_count);
                beatmap->m_hit_objects = (OsuHitObject *) malloc(sizeof(OsuHitObject) * beatmap->m_hit_object_count);
            }
            continue;
        }

        switch (current_section) {
            case General: HandleGeneralLine(beatmap, line); break;
            case Metadata: HandleMetadataLine(beatmap, line); break;
            case Difficulty: HandleDifficultyLine(beatmap, line); break;
            case Events: HandleEventLine(beatmap, line); break;
            case TimingPoints: HandleTimingPointLine(beatmap, line); break;
            case HitObjects: HandleHitObjectLine(beatmap, line); break;
            default: /* nothing */ break;
        }
    }

    fclose(in_file);

    printf("%zu/%zu timing points (=%zu bytes)\n", beatmap->m_timing_point_parsed, beatmap->m_timing_point_count, sizeof(OsuTimingPoint) * beatmap->m_timing_point_count);
    printf("%zu/%zu hit objects (>%zu bytes)\n", beatmap->m_hit_object_parsed, beatmap->m_hit_object_count, sizeof(OsuHitObject) * beatmap->m_hit_object_count);

    char real_path[1024];
    realpath(path, real_path);
    beatmap->SetRootPathFromFile(real_path);

    printf("beatmap root: %s\n", beatmap->GetRootPath());

    return beatmap;
}
