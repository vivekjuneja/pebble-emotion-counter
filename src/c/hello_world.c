#include <pebble.h>

// You can define defaults for values in persistent storage
#define NUM_HAPPY_DEFAULT 0
#define NUM_SAD_DEFAULT 0
#define NUM_ANGRY_DEFAULT 0


#define NUM_HAPPY_PKEY 1
#define NUM_SAD_PKEY 2
#define NUM_ANGRY_PKEY 3



static AppSync s_sync;
static uint8_t s_sync_buffer[64];

enum WeatherKey {
  WEATHER_ICON_KEY = 0x0,         // TUPLE_INT
  WEATHER_TEMPERATURE_KEY = 0x1,  // TUPLE_CSTRING
  WEATHER_CITY_KEY = 0x2,         // TUPLE_CSTRING
};

/*
static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_IMAGE_SUN, // 0
  RESOURCE_ID_IMAGE_CLOUD, // 1
  RESOURCE_ID_IMAGE_RAIN, // 2
  RESOURCE_ID_IMAGE_SNOW // 3
};
*/


typedef enum {
  QuoteKeyInit = 0,
  QuoteKeyFetch,
  QuoteKeySymbol,
  QuoteKeyPrice,
} QuoteKey;

static int s_happy = NUM_HAPPY_DEFAULT;

static int s_sad = NUM_SAD_DEFAULT;

static int s_angry = NUM_ANGRY_DEFAULT;

static int emotion_array[] = {0,0,0};

static const uint32_t ANIMAL_LOG_TAGS[3] = { 0x5, 0xb, 0xd }; // sealion, pelican, dolphin


static const GRect NUMBER_POSITIONS[3] =  {
  {{/* x: */ 80, /* y: */ 12 }, {/* width: */ 28, /* height: */ 28}},
  {{/* x: */ 80, /* y: */ 60 }, {/* width: */ 28, /* height: */ 28}},
  {{/* x: */ 80, /* y: */ 107 }, {/* width: */ 28, /* height: */ 28}}
};

typedef struct {
  uint32_t tag;
  TextLayer *text_layer;
  char text[20];
  DataLoggingSessionRef logging_session;
  int count;
  GBitmap *bitmap;
} AnimalData;

static Window *s_main_window;
static ActionBarLayer *s_action_bar_layer;
static AnimalData s_animal_data[3]; // 0 = sealion, 1 = dolphin, 2 = pelican
static TextLayer *s_intro_layer;


static GFont s_res_gothic_18_bold;
static GBitmap *s_res_happy_image, *s_res_sad_image, *s_res_angry_image;
static TextLayer *s_textlayer_1;
static BitmapLayer *s_bitmaplayer_1;
static BitmapLayer *s_bitmaplayer_2;
static BitmapLayer *s_bitmaplayer_3;


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case WEATHER_ICON_KEY:
      /*if (s_icon_bitmap) {
        gbitmap_destroy(s_icon_bitmap);
      }
      */
    
      //s_icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[new_tuple->value->uint8]);
      //bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
      //bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
      break;

    case WEATHER_TEMPERATURE_KEY:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
     // text_layer_set_text(s_textlayer_1, "DONE 1");
      break;

    case WEATHER_CITY_KEY:
     // text_layer_set_text(s_textlayer_1, "DONE 2");
      break;
  }
}

static void request_weather( const char *const cstring) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    return;
  }

  //int value = 1;
  dict_write_cstring(iter, QuoteKeyInit, cstring);
  dict_write_end(iter);

 app_message_outbox_send();
}



static void count_animal(AnimalData *animal_data) {
  animal_data->count++;
  time_t now = time(NULL);
  
  // Output log data to phone
  data_logging_log(animal_data->logging_session, (uint8_t *)&now, 1);
  
  snprintf(animal_data->text, 20, "%d", animal_data->count);
  text_layer_set_text(animal_data->text_layer, animal_data->text);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Ma, it's a dolphin!
  AnimalData *animal_data = &s_animal_data[0];
  count_animal(animal_data);
  s_happy = animal_data->count;
  printf("s_happy : %d", s_happy);
  
  app_message_open(64, 64);

 
      Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "1234\u00B0C"),
    TupletCString(WEATHER_CITY_KEY, "..."),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  
  
  request_weather("happy");


}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // A wonderful bird is the pelican
  // His bill will hold more than his belican
  // He can take in his beak
  // Food enough for a week,
  // But I'm damned if I see how the helican
  AnimalData *animal_data = &s_animal_data[1];
  count_animal(animal_data);
  s_sad = animal_data->count;
  printf("s_sad : %d", s_sad);
  
    app_message_open(64, 64);

    Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "1234\u00B0C"),
    TupletCString(WEATHER_CITY_KEY, "..."),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  
  
   request_weather("sad");


}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Oh, look daddy! A sea lion!
  AnimalData *animal_data = &s_animal_data[2];
  count_animal(animal_data);
  s_angry = animal_data->count;
  printf("s_angry : %d", s_angry);
  
    app_message_open(64, 64);

  
    Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "1234\u00B0C"),
    TupletCString(WEATHER_CITY_KEY, "..."),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  
  request_weather("angry");

}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  for (int i = 0; i < 3; i++) {
    AnimalData *animal_data = &s_animal_data[i];
    data_logging_finish(animal_data->logging_session);
    animal_data->count = 0;
    snprintf(animal_data->text, 20, "%d", animal_data->count);
    text_layer_set_text(animal_data->text_layer, animal_data->text);
    animal_data->logging_session = data_logging_create(ANIMAL_LOG_TAGS[i], DATA_LOGGING_UINT, 4, false);
  }
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 0, select_long_click_handler, NULL);
}

static void main_window_load(Window *s_window) {
  // Create ActionBar
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer, s_window);
  action_bar_layer_set_click_config_provider(s_action_bar_layer, config_provider);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_animal_data[0].bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_animal_data[1].bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_animal_data[2].bitmap);


  //s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 0);
  #endif
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_happy_image = gbitmap_create_with_resource(RESOURCE_ID_HAPPY_IMAGE);
  s_res_sad_image = gbitmap_create_with_resource(RESOURCE_ID_SAD_IMAGE);
  s_res_angry_image = gbitmap_create_with_resource(RESOURCE_ID_ANGRY_IMAGE);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(1, 71, 80, 20));
  text_layer_set_background_color(s_textlayer_1, GColorBlack);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "Emoter");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(115, 15, 26, 25));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_happy_image);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_bitmaplayer_2
  s_bitmaplayer_2 = bitmap_layer_create(GRect(115, 63, 26, 25));
  bitmap_layer_set_bitmap(s_bitmaplayer_2, s_res_sad_image);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_2);
  
  // s_bitmaplayer_3
  s_bitmaplayer_3 = bitmap_layer_create(GRect(115, 110, 26, 25));
  bitmap_layer_set_bitmap(s_bitmaplayer_3, s_res_angry_image);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_3);
  
  
 
}

static void main_window_unload(Window *window) {
  action_bar_layer_destroy(s_action_bar_layer);
  text_layer_destroy(s_intro_layer);
}

static void init() {  
  
  // Get the count from persistent storage for use if it exists, otherwise use the default
  s_happy = persist_exists(NUM_HAPPY_PKEY) ? persist_read_int(NUM_HAPPY_PKEY) : NUM_HAPPY_DEFAULT;
  s_sad = persist_exists(NUM_SAD_PKEY) ? persist_read_int(NUM_SAD_PKEY) : NUM_SAD_DEFAULT;
  s_angry = persist_exists(NUM_ANGRY_PKEY) ? persist_read_int(NUM_ANGRY_PKEY) : NUM_ANGRY_DEFAULT;
  
  // Create Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  printf("#######BEGIN: s_happy : %d , s_sad : %d, s_angry : %d", s_happy, s_sad, s_angry);
  emotion_array[0]= s_happy;
  emotion_array[1]= s_sad;
  emotion_array[2]= s_angry;
  

  // Init animal data
  for (int i = 0; i < 3; i++) {
    AnimalData *animal_data = &s_animal_data[i];
    animal_data->text_layer = text_layer_create(NUMBER_POSITIONS[i]);
    text_layer_set_text_color(animal_data->text_layer, GColorWhite );
    text_layer_set_background_color(animal_data->text_layer, GColorClear);
    layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(animal_data->text_layer));
    animal_data->logging_session = data_logging_create(ANIMAL_LOG_TAGS[i], DATA_LOGGING_UINT, 4, false);
    animal_data->bitmap = gbitmap_create_with_resource(RESOURCE_ID_SAD_IMAGE  );
    animal_data->count = emotion_array[i];
    snprintf(animal_data->text, 20, "%d", animal_data->count);
    text_layer_set_text(animal_data->text_layer, animal_data->text);
    text_layer_set_font(animal_data->text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  }
       
  
  


 
}

static void deinit() {
  
  int values_to_save[3];
  
  // Deinit animal data
  for (int i = 0; i < 3; i++) {
    AnimalData *animal_data = &s_animal_data[i];
    data_logging_finish(animal_data->logging_session);
    text_layer_destroy(animal_data->text_layer);
    gbitmap_destroy(animal_data->bitmap);
    values_to_save[i] = animal_data->count;
  }
  
  persist_write_int(NUM_HAPPY_PKEY, values_to_save[0]);
  persist_write_int(NUM_SAD_PKEY, values_to_save[1]);
  persist_write_int(NUM_ANGRY_PKEY, values_to_save[2]);
                                                 
  s_happy = values_to_save[0];
  s_sad = values_to_save[1];                                              
  s_angry = values_to_save[2];
                                                 
  printf("#######END: s_happy : %d , s_sad : %d, s_angry : %d", s_happy, s_sad, s_angry);

    app_sync_deinit(&s_sync);

  
  // Destroy Window
  window_destroy(s_main_window);
  
  
}





int main(void) {
  init();
  app_event_loop();
  deinit();
}