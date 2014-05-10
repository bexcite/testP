#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *time_layer;

char* fonts[] = {
  FONT_KEY_FONT_FALLBACK,
  FONT_KEY_GOTHIC_14,
  FONT_KEY_GOTHIC_14_BOLD,
  FONT_KEY_GOTHIC_18,
  FONT_KEY_GOTHIC_18_BOLD,
  FONT_KEY_GOTHIC_24,
  FONT_KEY_GOTHIC_24_BOLD,
  FONT_KEY_GOTHIC_28,
  FONT_KEY_GOTHIC_28_BOLD,
  /*FONT_KEY_BITHAM_30_BLACK,
  FONT_KEY_BITHAM_42_BOLD,
  FONT_KEY_BITHAM_42_LIGHT,
  FONT_KEY_BITHAM_42_MEDIUM_NUMBERS,
  FONT_KEY_BITHAM_34_MEDIUM_NUMBERS,
  FONT_KEY_BITHAM_34_LIGHT_SUBSET,
  FONT_KEY_BITHAM_18_LIGHT_SUBSET,
  FONT_KEY_ROBOTO_CONDENSED_21,
  FONT_KEY_ROBOTO_BOLD_SUBSET_49,
*/  
  FONT_KEY_DROID_SERIF_28_BOLD
};
uint16_t current_font_idx = 8; // FONT_KEY_GOTHIC_28_BOLD

static void select_click_handler(ClickRecognizerRef recognizer, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Select click 12");
  
  
  current_font_idx++;
  if (current_font_idx >= sizeof(fonts)/sizeof(fonts[0])) {
    current_font_idx = 0;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "current_font_idx %d", current_font_idx);
  //text_layer_set_font(time_layer, fonts_get_system_font(fonts[current_font_idx]));
  text_layer_set_font(time_layer, fonts_get_system_font(fonts[current_font_idx]));
  
  text_layer_set_text(text_layer, fonts[current_font_idx]);
  
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Up click");
}

static void down_click_handler(ClickRecognizerRef recognizer, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Down click");
}

static void back_click_handler(ClickRecognizerRef recognizer, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Back click");
}


static void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, &select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, &up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  //window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char time_text[] = "00:00:01";
  //APP_LOG(APP_LOG_LEVEL_DEBUG, time_text);
  
  strftime(time_text, sizeof(time_text), "%T", tick_time);
  text_layer_set_text(time_layer, time_text);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  
  char *str;
  if (units_changed && MINUTE_UNIT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Set Minutes");
    str = "Minute";
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Set Else");
    str = "else ...";
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "str = %s %p", str, &str);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "units_changed = %x %p", units_changed, &units_changed);
  
}

void init(void) {
  window = window_create();
  window_stack_push(window, false);
  window_set_click_config_provider(window, &config_provider);
  

  Layer* window_layer = window_get_root_layer(window);
  
  // Create a text_layer  
  text_layer = text_layer_create(GRect(0, 0, 144, 144));
  text_layer_set_background_color(text_layer, GColorWhite);
  
  //Get some info about layers
  GRect bounds = layer_get_bounds(text_layer_get_layer(text_layer));
  char *log_str = (char *) malloc(sizeof(char) * 180);
  snprintf(log_str, 180, "text_layer.bounds = x%d y%d w%d h%d", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "text_layer.bounds = x%d y%d", bounds.origin.x, bounds.origin.y);
  
  
  text_layer_set_text(text_layer, log_str);
  strcat(log_str, ", ");
  strcat(log_str, fonts[current_font_idx]);
  //text_layer_set_text(text_layer, fonts[current_font_idx]);
  
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  
  // Create a time_layer
  time_layer = text_layer_create(GRect(29, 84, 144-40, 168-54));
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(fonts[current_font_idx]));
  
  //text_layer_set_text(time_layer, "00:00:00");
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_second_tick(current_time, SECOND_UNIT);
  
//  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  
  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
  
  layer_add_child(window_get_root_layer(window), (Layer *)time_layer);
  

  
  //tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
}


void deinit(void) {
  text_layer_destroy(text_layer);
  text_layer_destroy(time_layer);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
