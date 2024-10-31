#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"
#include "esp_timer.h"

static const char *TAG = "Touch pad";

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_THRESH_PERCENT  (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)
#define DISPLAY_REFRESH_TIME 1000 // Tempo de atualização do display (1 segundo)

// Definição dos sensores e deadlines
#define SENSOR_INJECAO_PAD 0
#define SENSOR_TEMP_PAD 6
#define SENSOR_ABS_PAD 5
#define SENSOR_AIRBAG_PAD 3
#define SENSOR_CINTO_PAD 4

#define DEADLINE_INJECAO 0.5       // 500 us
#define DEADLINE_TEMP 20           // 20 ms
#define DEADLINE_ABS 100           // 100 ms
#define DEADLINE_AIRBAG 100        // 100 ms
#define DEADLINE_CINTO 1000        // 1 s

static bool s_pad_activated[TOUCH_PAD_MAX];
static uint32_t s_pad_init_val[TOUCH_PAD_MAX];

// Configuração dos thresholds
static void tp_example_set_thresholds(void) {
    uint16_t touch_value;
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        touch_pad_read_filtered(i, &touch_value);
        s_pad_init_val[i] = touch_value;
        ESP_LOGI(TAG, "Touch pad [%d] initial value is %d", i, touch_value);
        ESP_ERROR_CHECK(touch_pad_set_thresh(i, touch_value * 2 / 3));
    }
}

// Função para exibir o estado dos sensores
void exibir_estado_sensores() {
    //touch_pad_intr_enable();
    printf("\n---- Estado dos Sensores ----\n");
    printf("Injeção Eletrônica: %s\n", s_pad_activated[SENSOR_INJECAO_PAD] ? "Ativado" : "Desativado");
    printf("Temperatura: %s\n", s_pad_activated[SENSOR_TEMP_PAD] ? "Ativado" : "Desativado");
    printf("ABS: %s\n", s_pad_activated[SENSOR_ABS_PAD] ? "Ativado" : "Desativado");
    printf("Airbag: %s\n", s_pad_activated[SENSOR_AIRBAG_PAD] ? "Ativado" : "Desativado");
    printf("Cinto de Segurança: %s\n", s_pad_activated[SENSOR_CINTO_PAD] ? "Ativado" : "Desativado");
    printf("-----------------------------\n");
}

// Função para tratar e calcular o tempo de execução de cada sensor
static void laco_tratador_de_interrupcao(void *pvParameter) {
    uint64_t eus, eus2, eus3, eus4, eus5, eus6, eus7;
    touch_pad_intr_enable();
    
    while (1) {

        for (int i = 0; i < TOUCH_PAD_MAX; i++) {
            if (s_pad_activated[i] == true) {
                eus = esp_timer_get_time();  // Marca o tempo de início

                switch (i) {
                    case SENSOR_INJECAO_PAD:
                        printf("Sensor Injeção Eletrônica ativado, deadline: %.1f us\n", DEADLINE_INJECAO);
                        eus2 = esp_timer_get_time();  // Marca o tempo de término
                        printf("Tempo de execução para o sensor %d: %llu us\n", i, (eus2 - eus));
                        vTaskDelay(1 / portTICK_PERIOD_MS);  
                        break;
                    case SENSOR_TEMP_PAD:
                        printf("Sensor Temperatura ativado, deadline: %d ms\n", DEADLINE_TEMP);
                        eus3 = esp_timer_get_time();  // Marca o tempo de término
                        printf("Tempo de execução para o sensor %d: %llu us\n", i, (eus3 - eus));
                        vTaskDelay(20 / portTICK_PERIOD_MS);  
                        break;
                    case SENSOR_ABS_PAD:
                        printf("Sensor ABS ativado, deadline: %d ms\n", DEADLINE_ABS);
                        eus4 = esp_timer_get_time();  // Marca o tempo de término
                        printf("Tempo de execução para o sensor %d: %llu us\n", i, (eus4 - eus));
                        vTaskDelay(100 / portTICK_PERIOD_MS);  
                        break;
                    case SENSOR_AIRBAG_PAD:
                        printf("Sensor Airbag ativado, deadline: %d ms\n", DEADLINE_AIRBAG);
                        eus5 = esp_timer_get_time();  // Marca o tempo de término
                        printf("Tempo de execução para o sensor %d: %llu us\n", i, (eus5 - eus));
                        vTaskDelay(100 / portTICK_PERIOD_MS);  
                        break;
                    case SENSOR_CINTO_PAD:
                        printf("Sensor Cinto de Segurança ativado, deadline: %d ms\n", DEADLINE_CINTO);
                        eus6 = esp_timer_get_time();  // Marca o tempo de término
                        printf("Tempo de execução para o sensor %d: %llu us\n", i, (eus6 - eus));
                        vTaskDelay(1000 / portTICK_PERIOD_MS);  
                        break;
                    default:
                        printf("Sensor desconhecido ativado.\n");
                        break;
                }
                //aqui vai o display
                exibir_estado_sensores(); 
                eus7 = esp_timer_get_time();  // Marca o tempo de término
                printf("Tempo de execução TASK COMPLETA %llu us\n",(eus7 - eus));

                s_pad_activated[i] = false;  // Reseta o estado de ativação do sensor
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));  // Tempo base de execução do loop principal
    }
}


void task_injecao(void *pvParameter) {
    uint64_t eus, eus2;
    touch_pad_intr_enable();
    while (1) {
        if (s_pad_activated[SENSOR_INJECAO_PAD]) {
            eus = esp_timer_get_time();
            printf("Sensor Injeção Eletrônica ativado, deadline: %.1f us\n", DEADLINE_INJECAO);
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor Injeção: %llu us\n", (eus2 - eus));
            vTaskDelay(1 / portTICK_PERIOD_MS);  // Simulação de processamento
            s_pad_activated[SENSOR_INJECAO_PAD] = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void task_temperatura(void *pvParameter) {
    uint64_t eus, eus2;
    touch_pad_intr_enable();
    while (1) {
        if (s_pad_activated[SENSOR_TEMP_PAD]) {
            eus = esp_timer_get_time();
            printf("Sensor Temperatura ativado, deadline: %d ms\n", DEADLINE_TEMP);
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor Temperatura: %llu us\n", (eus2 - eus));
            vTaskDelay(20 / portTICK_PERIOD_MS);  // Simulação de processamento
            s_pad_activated[SENSOR_TEMP_PAD] = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void task_abs(void *pvParameter) {
    uint64_t eus, eus2;
    touch_pad_intr_enable();
    while (1) {
        if (s_pad_activated[SENSOR_ABS_PAD]) {
            eus = esp_timer_get_time();
            printf("Sensor ABS ativado, deadline: %d ms\n", DEADLINE_ABS);
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor ABS: %llu us\n", (eus2 - eus));
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Simulação de processamento
            s_pad_activated[SENSOR_ABS_PAD] = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void task_airbag(void *pvParameter) {
    uint64_t eus, eus2;
    touch_pad_intr_enable();
    while (1) {        
        if (s_pad_activated[SENSOR_AIRBAG_PAD]) {
            eus = esp_timer_get_time();
            printf("Sensor Airbag ativado, deadline: %d ms\n", DEADLINE_AIRBAG);
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor Airbag: %llu us\n", (eus2 - eus));
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Simulação de processamento
            s_pad_activated[SENSOR_AIRBAG_PAD] = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void task_cinto(void *pvParameter) {
    uint64_t eus, eus2;
        touch_pad_intr_enable();
    while (1) {
        
            eus = esp_timer_get_time();
        if (s_pad_activated[SENSOR_CINTO_PAD]) {
            printf("Sensor Cinto de Segurança ativado, deadline: %d ms\n", DEADLINE_CINTO);
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor Cinto: %llu us\n", (eus2 - eus));
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Simulação de processamento
            s_pad_activated[SENSOR_CINTO_PAD] = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void task_display(void *pvParameter) {
    uint64_t eus, eus2;
    touch_pad_intr_enable();
    while (1) {
        eus = esp_timer_get_time();
        // Exibir os estados dos sensores
        printf("\n---- Estado dos Sensores ----\n");
        printf("Atualização do Display:\n");
        printf("Injeção Eletrônica: %s\n", s_pad_activated[SENSOR_INJECAO_PAD] ? "Ativado" : "0");
        printf("Temperatura: %s\n", s_pad_activated[SENSOR_TEMP_PAD] ? "Ativado" : "0");
        printf("ABS: %s\n", s_pad_activated[SENSOR_ABS_PAD] ? "Ativado" : "0");
        printf("Airbag: %s\n", s_pad_activated[SENSOR_AIRBAG_PAD] ? "Ativado" : "0");
        printf("Cinto de Segurança: %s\n", s_pad_activated[SENSOR_CINTO_PAD] ? "Ativado" : "0");
        printf("-----------------------------\n");

        // Resetar sensores após o tempo especificado
        for (int i = 0; i < TOUCH_PAD_MAX; i++) {
            if (s_pad_activated[i]) {
                s_pad_activated[i] = false;  // Desativar sensor se o tempo limite foi atingido
            }
        }
            eus2 = esp_timer_get_time();
            printf("Tempo de execução para o Sensor Cinto: %llu us\n", (eus2 - eus));
        // Atrasar a tarefa por 1 segundo antes de atualizar novamente
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Interrupção para ativação dos sensores
static void tp_example_rtc_intr(void *arg) {
    uint32_t pad_intr = touch_pad_get_status();
    touch_pad_clear_status();
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        if ((pad_intr >> i) & 0x01) {
            s_pad_activated[i] = true;
        }
    }
}

// Configuração do touch pad
static void touch_init(void) {
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        touch_pad_config(i, TOUCH_THRESH_NO_USE);
    }
}

// Função principal
void app_main(void) {
    ESP_LOGI(TAG, "iniciando touch pad");
    touch_pad_init();
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_init();
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    tp_example_set_thresholds();
    touch_pad_isr_register(tp_example_rtc_intr, NULL);

    //laço com interrupcao

    //xTaskCreate(&laco_tratador_de_interrupcao, "Laco task", 2048, NULL, 5, NULL);

    // multi task

    //xTaskCreate(&task_injecao, "Task Injeção", 2048, NULL, 3, NULL);
    //xTaskCreate(&task_temperatura, "Task Temperatura", 2048, NULL, 2, NULL);
    //xTaskCreate(&task_abs, "Task ABS", 2048, NULL, 4, NULL);
    //xTaskCreate(&task_airbag, "Task Airbag", 2048, NULL, 5, NULL);
    //xTaskCreate(&task_cinto, "Task Cinto", 2048, NULL, 1, NULL);

    // Criação da tarefa de display
    //xTaskCreate(&task_display, "Task Display", 2048, NULL, 5, NULL);
}