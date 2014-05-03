#include <pebble.h>

Window *window;
TextLayer *text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Select click");
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
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  
  char *str;
  if (units_changed == MINUTE_UNIT) {
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
  window_stack_push(window, true);
  window_set_click_config_provider(window, &config_provider);
  

  text_layer = text_layer_create(GRect(0, 0, 144, 20));

  
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
}


void deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
