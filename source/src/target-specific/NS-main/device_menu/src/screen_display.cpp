#include "mbed.h"
#include "screen_display.h"
#include "menu_service.h"
#include "DataService.h"
// #include "error_controller.h"
#include "consts.h"

#define SCREEN_WIDTH              127
#define SCREEN_HEIGHT             63
#define SCREEN_CENTER_HORIZONTAL  (SCREEN_WIDTH/2+1)
#define SCREEN_CENTER_VERTICAL    (SCREEN_HEIGHT/2+1)
#define ICON_L_SIZE               20
#define ICON_S_SIZE               12
#define FONT_TRANSPARENT          1
#define FONT_NON_TRANSPARENT      0

u8g2_t myScreen;

uint8_t u8g2_byte_hw_i2c_mbed(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_gpio_and_delay_mbed(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

namespace ScreenDisplay {
  uint8_t loadingState;
  
  void setup() {
    loadingState = 0;
    init();
    clear();
  }
	
	void init() {
		u8g2_Setup_ssd1306_i2c_128x64_noname_f(&myScreen, U8G2_R0, u8g2_byte_hw_i2c_mbed, u8g2_gpio_and_delay_mbed);
    u8g2_InitDisplay(&myScreen);
    u8g2_SetPowerSave(&myScreen, 0);
    u8g2_SetFontMode(&myScreen, FONT_TRANSPARENT);
		u8g2_SendBuffer(&myScreen);
	}
  
  void clear() {
    u8g2_ClearBuffer(&myScreen);
  }
  
  void render(ScreenModel *screen) {
    uint16_t w;
    int16_t unit_offset = 0;
    int16_t h_offset = 0;

    char textString[16];
    uint8_t r;
    
    clear();
    
    if(menu_actions::isLoading()) {
      u8g2_SetFont(&myScreen, SmallFont);
      u8g2_SetFontPosTop(&myScreen);
      w = u8g2_GetStrWidth(&myScreen, NECTAR_DEVICE_MODEL);
      u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 0, NECTAR_DEVICE_MODEL);
			
			u8g2_SetFont(&myScreen, XSmallFont);
      u8g2_SetFontPosTop(&myScreen);
      switch(loadingState) {
        case 0:
          w = u8g2_GetStrWidth(&myScreen, "Initializing device...");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "Initializing device...");
          break;
        
				case 1:
          w = u8g2_GetStrWidth(&myScreen, "Starting Power comms...");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "Starting Power comms...");
          break;
				
				case 2:
					w = u8g2_GetStrWidth(&myScreen, "Power comms started");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "Power comms started");
					break;
				
        case 3:
          w = u8g2_GetStrWidth(&myScreen, "Starting ESP comms...");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "Starting ESP comms...");
          break;
				
				case 4:
          w = u8g2_GetStrWidth(&myScreen, "ESP comms started");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "ESP comms started");
          break;
				
				case 5:
					w = u8g2_GetStrWidth(&myScreen, "Starting device...");
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 20, "Starting device...");
          break;
      }
      
      u8g2_SetFont(&myScreen, XSmallFont);    
      u8g2_SetFontPosBottom(&myScreen);
			switch(loadingState) {
				case 0:
				case 1:
					snprintf(textString, 2, " ");
					break;					
				
				case 2:
				case 3:
					snprintf(textString, 12, "Power v%1.2f", powerData.power_version/100);
					w = u8g2_GetStrWidth(&myScreen, textString);
					u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_HEIGHT-10, textString);
					break;
				
				case 4:
				case 5:
					snprintf(textString, 12, "Power v%1.2f", powerData.power_version/100);
					w = u8g2_GetStrWidth(&myScreen, textString);
					u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_HEIGHT-10, textString);
					
					snprintf(textString, 10, "ESP v%1.2f", espData.esp_version/100);
					w = u8g2_GetStrWidth(&myScreen, textString);
					u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_HEIGHT, textString);
					break;
			}			
			snprintf(textString, 6, "v%1.2f", NECTAR_MAIN_BOARD_VERSION/100);
			w = u8g2_GetStrWidth(&myScreen, textString);
			u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_HEIGHT-20, textString);
			
      loadingState++;
    } else {
      
      if(menu_actions::isInTestMode()) {
        u8g2_SetFont(&myScreen, XSmallFont);
        u8g2_SetFontPosTop(&myScreen);
        w = u8g2_GetStrWidth(&myScreen, "TEST MODE");
        u8g2_DrawStr(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 0, "TEST MODE");
      }
      
      switch(screen->screenType) {
        
    /*
    DEBUG SCREEN
    */
        case Debug:
          u8g2_SetFont(&myScreen, XSmallFont);
          snprintf(textString, 25, "%.2fV %.2fA %.2f", powerData.sun_voltage, powerData.sun_current, powerData.pwm_duty);
          u8g2_DrawStr(&myScreen, 0, 10, textString);
          if(powerData.device_calibrated) {
            snprintf(textString, 20, "%fV %fA", powerData.ref_voltage, powerData.ref_current);
            u8g2_DrawStr(&myScreen, 0, 18, textString);
          } else {
            u8g2_DrawStr(&myScreen, 0, 18, "Not calibrated");
          }
          
          snprintf(textString, 25, "boiler: %.2f", temperatureData.getBoilerTemperature());
          u8g2_DrawStr(&myScreen, 0, 30, textString);
          snprintf(textString, 27, "internal: %.2f", temperatureData.getDeviceTemperature());
          u8g2_DrawStr(&myScreen, 0, 38, textString);

          if(DataService::isSunRelayOn()) {
            u8g2_DrawStr(&myScreen, 0, 50, "sun relay ON");
          } else if(DataService::isGridRelayOn()) {
            u8g2_DrawStr(&myScreen, 0, 50, "grid relay ON");
          } else {
            u8g2_DrawStr(&myScreen, 0, 50, "both relays OFF");
          }
        break;
          
    /*
    DEBUG ERROR SCREEN
    */  
        case DebugError:
        //   u8g2_SetFont(&myScreen, XSmallFont);
        //   snprintf(textString, 25, "main error: %d", mainBoardError.get_errors());
        //   u8g2_DrawStr(&myScreen, 0, 10, textString);
        //   snprintf(textString, 20, "power error: %d", powerBoardError.get_errors());
        //   u8g2_DrawStr(&myScreen, 0, 18, textString);
				
					snprintf(textString, 17, "Main Board v%1.2f", NECTAR_MAIN_BOARD_VERSION/100);
          u8g2_DrawStr(&myScreen, 0, 36, textString);
					snprintf(textString, 18, "Power Board v%1.2f", powerData.power_version/100);
          u8g2_DrawStr(&myScreen, 0, 44, textString);
					snprintf(textString, 10, "ESP v%1.2f", espData.esp_version/100);
          u8g2_DrawStr(&myScreen, 0, 52, textString);
        break;

    /*
    IDLE SCREEN
    */
        case Idle:
          r = 6;
          u8g2_SetFont(&myScreen, XLargeFont);
          u8g2_SetFontPosTop(&myScreen);
          snprintf(textString, 5, "%.0f", temperatureData.getBoilerTemperature());
          w = u8g2_GetUTF8Width(&myScreen, textString);
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2 - 7, 0, textString);
          u8g2_DrawCircle(&myScreen, SCREEN_CENTER_HORIZONTAL + w/2 + 7 + r, r, r, U8G2_DRAW_ALL);
          u8g2_DrawCircle(&myScreen, SCREEN_CENTER_HORIZONTAL + w/2 + 7 + r, r, r-1, U8G2_DRAW_ALL);
        break;
        
      /*
    IDLE ERROR SCREEN
    */  
        case IdleError:
          u8g2_SetFont(&myScreen, SmallFont);
          u8g2_SetFontPosTop(&myScreen);
          w = u8g2_GetUTF8Width(&myScreen, screen->title.c_str());
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, 10, screen->title.c_str());

          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_SetFontPosCenter(&myScreen);
          w = u8g2_GetUTF8Width(&myScreen, screen->subtitle.c_str());
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_CENTER_VERTICAL + 5, screen->subtitle.c_str());
        
          if(menu_actions::isInTestMode()) {
//            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL - 10 + h_offset, SCREEN_HEIGHT - 4, SCREEN_CENTER_HORIZONTAL + h_offset, SCREEN_HEIGHT);
//            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL + h_offset, SCREEN_HEIGHT, SCREEN_CENTER_HORIZONTAL + 10 + h_offset, SCREEN_HEIGHT - 4);
          } else {
            u8g2_SetFont(&myScreen, XSmallFont);
            u8g2_SetFontPosBottom(&myScreen);
            w = u8g2_GetUTF8Width(&myScreen, "PLEASE RESTART");
            u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_HEIGHT, "PLEASE RESTART");
          }
        break;

    /*
    ACTIVE STATUS SCREEN
    */      
        case ActiveStatus:
          r = 2;
          u8g2_SetFont(&myScreen, LargeFont);
          u8g2_SetFontPosTop(&myScreen);
        
          snprintf(textString, 5, "%.0f", temperatureData.getBoilerTemperature());
          w = u8g2_GetUTF8Width(&myScreen, textString);
          u8g2_DrawUTF8(&myScreen, 0, 0, textString);
          u8g2_DrawCircle(&myScreen, w + 5 + r, r, r, U8G2_DRAW_ALL);
        
          if(DataService::isSunRelayOn() && (powerData.sun_power > 5.00)) {
            u8g2_SetFont(&myScreen, LargeFont);
            u8g2_SetFontPosTop(&myScreen);
            if(powerData.sun_power > 1000) {
              w = u8g2_GetUTF8Width(&myScreen, "kW");
              unit_offset = w + 1;
              u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, 0, "kW");
              
              snprintf(textString, 5, "%.2f", powerData.sun_power/1000);
            } else {
              w = u8g2_GetUTF8Width(&myScreen, "W");
              unit_offset = w;
              u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, 0, "W");
              
              snprintf(textString, 5, "%.0f", powerData.sun_power);
            }
            w = u8g2_GetUTF8Width(&myScreen, textString);
            u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset - w - 5, 0, textString);
          }
          
          u8g2_SetFont(&myScreen, SmallFont);
          u8g2_SetFontPosBaseline(&myScreen);
          
          if(!deviceOpMode_isConfigured()) {
            switch(DataService::getCurrentHeaterMode()) {
              default:
              case nectar_contract::None:
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_AUTO)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationDefault).c_str());
                break;
              
              case nectar_contract::Boost:
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_BOOST)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationBoost).c_str());
                break;
              
              case nectar_contract::Nogrid:
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_NO_GRID)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationNoGrid).c_str());
                break;
              
              case nectar_contract::Away:
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_AWAY)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationAway).c_str());
                break;
            }
          } else {
            if(espData.has_internet_connection) {
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_CONNECTED)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationConnected).c_str());
            } else {
                u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_NOT_CONNECTED)));
                u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT-4, LocalizedString(LocalizationNotConnected).c_str());
            }
          }
          
          u8g2_SetFont(&myScreen, SmallFont);
          u8g2_SetFontPosBaseline(&myScreen);
					
					// if(menu_actions::hasErrors()) {
					// 	if(powerBoardError.has_errors) {
					// 		snprintf(textString, 5, "%d", powerBoardError.get_errors());
					// 	} else if(mainBoardError.has_errors) {
					// 		snprintf(textString, 5, "%d", mainBoardError.get_errors());
					// 	}
					// 	u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - ICON_S_SIZE, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_WARNING)));
					// 	w = u8g2_GetUTF8Width(&myScreen, textString);
					// 	u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - w, SCREEN_HEIGHT-4, textString);
					// } else {
						if(DataService::isSunRelayOn()) {
							if(powerData.sun_power > 5.00) {
								u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - ICON_S_SIZE, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_SUN)));
								w = u8g2_GetUTF8Width(&myScreen, LocalizedString(LocalizationSun).c_str());           
								u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - w, SCREEN_HEIGHT-4, LocalizedString(LocalizationSun).c_str());
							} else {
								u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - ICON_S_SIZE, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_IDLE)));
								w = u8g2_GetUTF8Width(&myScreen, LocalizedString(LocalizationIdle).c_str());           
								u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - w, SCREEN_HEIGHT-4, LocalizedString(LocalizationIdle).c_str());
							}
						} else if(DataService::isGridRelayOn()) {
							u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - ICON_S_SIZE, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_GRID)));
							w = u8g2_GetUTF8Width(&myScreen, LocalizedString(LocalizationGrid).c_str());           
							u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - w, SCREEN_HEIGHT-4, LocalizedString(LocalizationGrid).c_str());
						} else {
							u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - ICON_S_SIZE, SCREEN_CENTER_VERTICAL-4, 2, ICON_S_SIZE, get_icon(ICON(ICON_SMALL_IDLE)));
							w = u8g2_GetUTF8Width(&myScreen, LocalizedString(LocalizationIdle).c_str());           
							u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - w, SCREEN_HEIGHT-4, LocalizedString(LocalizationIdle).c_str());
						}
					// }
          break;

    /*
    ACTIVE TOTAL SCREEN
    */
        case ActiveTotal:
          u8g2_SetFont(&myScreen, SmallFont);
          u8g2_SetFontPosTop(&myScreen);

          u8g2_DrawUTF8(&myScreen, 0, 3, LocalizedString(LocalizationToday).c_str());
          u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT/2+3, LocalizedString(LocalizationTotal).c_str());
          
          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_SetFontPosBaseline(&myScreen);
          
          unit_offset = u8g2_GetUTF8Width(&myScreen, " Wh");
          u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, SCREEN_HEIGHT/2, " Wh");
          snprintf(textString, 10, "%.0f", DataService::getSolarKwhToday()*1000);
          w = u8g2_GetUTF8Width(&myScreen, textString);
          u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset - w, SCREEN_HEIGHT/2, textString);
        
          if(powerData.sun_meter_kwh >= 1000) {
            unit_offset = u8g2_GetUTF8Width(&myScreen, " MWh");
            u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, SCREEN_HEIGHT, " MWh");
            snprintf(textString, 10, "%.3f", powerData.sun_meter_kwh/1000);

          } else if(powerData.sun_meter_kwh >= 1) {
            unit_offset = u8g2_GetUTF8Width(&myScreen, " kWh");
            u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, SCREEN_HEIGHT, " kWh");
            snprintf(textString, 10, "%.2f", powerData.sun_meter_kwh);
            
          } else {
            unit_offset = u8g2_GetUTF8Width(&myScreen, " Wh");
            u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset, SCREEN_HEIGHT, " Wh");
            snprintf(textString, 10, "%.0f", powerData.sun_meter_kwh*1000);
          }
          w = u8g2_GetUTF8Width(&myScreen, textString);
          u8g2_DrawUTF8(&myScreen, SCREEN_WIDTH - unit_offset - w, SCREEN_HEIGHT, textString);
        break;

    /* 
    TITLED SCREENS
    */
        case Titled:
          if(screen->icon != ICON_NONE) {
            h_offset += ICON_L_SIZE + 8;
            u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL - ICON_L_SIZE/2, 3, ICON_L_SIZE, get_icon(screen->icon));
          }
          
          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_SetFontPosCenter(&myScreen);
          u8g2_DrawUTF8(&myScreen, h_offset, SCREEN_CENTER_VERTICAL +2, screen->title.c_str());
        break;
        
    /* 
    SUBTITLED SCREENS
    */
        case Subtitled:
          if(screen->icon != ICON_NONE) {
            h_offset += ICON_L_SIZE + 8;
            u8g2_DrawBitmap(&myScreen, 0, SCREEN_CENTER_VERTICAL - ICON_L_SIZE/2, 3, ICON_L_SIZE, get_icon(screen->icon));
          }
          
          u8g2_SetFont(&myScreen, SmallFont);
          u8g2_SetFontPosTop(&myScreen);
          u8g2_DrawUTF8(&myScreen, h_offset, 0, screen->title.c_str());

          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_SetFontPosCenter(&myScreen);
          u8g2_DrawUTF8(&myScreen, h_offset, SCREEN_CENTER_VERTICAL +2, screen->subtitle.c_str());
        break;

    /* 
    SELECTOR SCREENS
    */    
        case Selector:
          if(screen->selector.numberOfItems > 1) {
            if(screen->selector.selected == 0) {
              h_offset = -18;
            } else {
              h_offset = +21;
            }
          }
          
          //oled.drawLine(SCREEN_WIDTH/2 - 15 + h_offset, 12, SCREEN_WIDTH/2 + 15 + h_offset, 12, WHITE);
          //oled.drawLine(SCREEN_WIDTH/2 - 15 + h_offset, SCREEN_HEIGHT - 12, SCREEN_WIDTH/2 + 15 + h_offset, SCREEN_HEIGHT - 12, WHITE);
          
          if(screen->hasUpModel) {
            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL - 10 + h_offset, 4, SCREEN_CENTER_HORIZONTAL + h_offset, 0);
            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL + h_offset, 0, SCREEN_CENTER_HORIZONTAL + 10 + h_offset, 4);
          }
          
          if(screen->hasDownModel) {
            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL - 10 + h_offset, SCREEN_HEIGHT - 4, SCREEN_CENTER_HORIZONTAL + h_offset, SCREEN_HEIGHT);
            u8g2_DrawLine(&myScreen, SCREEN_CENTER_HORIZONTAL + h_offset, SCREEN_HEIGHT, SCREEN_CENTER_HORIZONTAL + 10 + h_offset, SCREEN_HEIGHT - 4);
          }
          

          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_SetFontPosCenter(&myScreen);
          w = u8g2_GetUTF8Width(&myScreen, screen->title.c_str());
          u8g2_DrawUTF8(&myScreen, SCREEN_CENTER_HORIZONTAL - w/2, SCREEN_CENTER_VERTICAL +2, screen->title.c_str());
          
        break;

    /* 
    BLANK SCREENS
    */
        default:
          u8g2_SetFont(&myScreen, MediumFont);
          u8g2_DrawUTF8(&myScreen, 0, SCREEN_HEIGHT/2, screen->title.c_str());
        break;
      }
      
      if(screen->hasNextModel && (screen->icon != ICON_BACK) && (screen->screenType != Idle)) {
        u8g2_DrawBitmap(&myScreen, SCREEN_WIDTH - 6, SCREEN_CENTER_VERTICAL - ICON_L_SIZE/2, 1, ICON_L_SIZE, get_icon(ICON_FORWARD));
      }
    }
      
    __disable_irq();
    u8g2_SendBuffer(&myScreen);
    __enable_irq();
    printf("SCREEN %s\n", screen->title.c_str());
  }
}
