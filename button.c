/**
 * Embarcatech 
 * Exemplo Botão com a BitDogLab
 */

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <ctype.h>
 #include "pico/stdlib.h"
 #include "pico/binary_info.h"
 #include "inc/ssd1306.h"
 #include "hardware/i2c.h"
 
 #include <math.h>
 #include "hardware/adc.h"
 
 #include "hardware/dma.h"
  #include "inc/data.h"
 
 const uint I2C_SDA = 14;
 const uint I2C_SCL = 15;

 #define LED_BLUE 12   // GPIO conectado ao terminal azul do LED RGB
 #define BUTTON_A 5    // GPIO conectado ao Botão A
 #define BUTTON_B 6    // GPIO conectado ao Botão A
 
 #define ROWS 50  // Número máximo de palavras
 #define COLS 20 // Tamanho máximo de cada palavra

 int myFunction4(int a) {
    return a;
 }

 /*float mic_power();
 */

 int adicao = 0;
 int adicao2 = 0;


// Declare two functions, myFunction and myOtherFunction
/*int myFunction();
int myOtherFunction();
uint8_t get_intensity(float v);
*/

 int main() {

  
  
    //myFunction(); // call myFunction (from main)
    

     // Configuração do GPIO do LED como saída
     gpio_init(LED_BLUE);
     gpio_set_dir(LED_BLUE, GPIO_OUT);
     gpio_put(LED_BLUE, false);  // Inicialmente, o LED está apagado
 
     // Configuração do GPIO do Botão A como entrada com pull-up interno
     gpio_init(BUTTON_A);
     gpio_set_dir(BUTTON_A, GPIO_IN);
     gpio_pull_up(BUTTON_A);

     // Configuração do GPIO do Botão B como entrada com pull-up interno
     gpio_init(BUTTON_B);
     gpio_set_dir(BUTTON_B, GPIO_IN);
     gpio_pull_up(BUTTON_B);


    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização do GPIO Goertzel
    //adc_init();
    //adc_gpio_init(28);   // Configura GPIO 28 como entrada do ADC
    //adc_select_input(2); // Usa o canal 2 do ADC

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);


    
    char dst[count_of(src)];

   /* void sample_mic() {
      adc_fifo_drain(); // Limpa o FIFO do ADC.
      adc_run(false); // Desliga o ADC (se estiver ligado) para configurar o DMA.
    
      dma_channel_configure(dma_channel, &dma_cfg,
        adc_buffer, // Escreve no buffer.
        &(adc_hw->fifo), // Lê do ADC.
        SAMPLES, // Faz SAMPLES amostras.
        true // Liga o DMA.
      );
    
      // Liga o ADC e espera acabar a leitura.
      adc_run(true);
      dma_channel_wait_for_finish_blocking(dma_channel);
      
      // Acabou a leitura, desliga o ADC de novo.
      adc_run(false);
    }*/

    //int matrix9[4][3] = { {1, 4, 2}, {3, 6, 8}, {1, 5, 2}, {2, 6, 8} };
    //return matrix[b][0];

// Get a free channel, panic() if there are none
int chan = dma_claim_unused_channel(true);

// 8 bit transfers. Both read and write address increment after each
// transfer (each pointing to a location in src or dst respectively).
// No DREQ is selected, so the DMA transfers as fast as it can.

dma_channel_config c = dma_channel_get_default_config(chan);
channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
channel_config_set_read_increment(&c, true);
channel_config_set_write_increment(&c, true);


ssd1306_draw_line(ssd, 0, 0, 120, 0, true);
             ssd1306_draw_line(ssd, 0, 18, 40, 18, true);
             ssd1306_draw_line(ssd, 40, 18, 40, 0, true);
             ssd1306_draw_line(ssd, 0, 63, 120, 63, true);

while (true) {

  
    adicao2 += 1;

    dma_channel_configure(
        chan,          // Channel to be configured
        &c,            // The configuration we just created
        dst,           // The initial write address
        src,           // The initial read address
        500, // Number of transfers; in this case each is 1 byte.
        true           // Start immediately.
        );
        
        // We could choose to go and do something else whilst the DMA is doing its
        // thing. In this case the processor has nothing else to do, so we just
        // wait for the DMA to finish.
        
        dma_channel_wait_for_finish_blocking(chan);
        dma_channel_cleanup(chan);
        dma_channel_abort(chan);
        dma_channel_abort(chan);
        // The DMA has now copied our text from the transmit buffer (src) to the
        // receive buffer (dst), so we can print it out from there.
        //puts(dst);

        //char str[] = "C e uma linguagem poderosa";
    char array[ROWS][COLS]; // Array 2D de palavras
    char *token;
    int i = 0;

    // Dividir a string em palavras
    token = strtok(dst, " ");
    while (token != NULL && i < ROWS) {
        strncpy(array[i], token, COLS - 1); // Copiar palavra para o array
        array[i][COLS - 1] = '\0'; // Garantir terminação nula
        i++;
        token = strtok(NULL, " ");
    }


         // Lê o estado do Botão A
         bool button_a_state = gpio_get(BUTTON_A);  // HIGH = solto, LOW = pressionado
         bool button_b_state = gpio_get(BUTTON_B);
         // Atualiza o estado do LED com base no estado do Botão A
         gpio_put(LED_BLUE, button_a_state);  // Se solto (HIGH), LED acende; se pressionado (LOW), apaga

         //float avg = mic_power();


         /*char str1[] = "Hello World!";
  char str2[] = "Write code!";
  strncpy(dst, str1, 6);
  printf("%s\n", str1);
  printf("%s\n", str2);
*/

  //printf("Array de palavras:\n");
    /*for (int j = 0; j < i; j++) {
        printf("%s\n", array[j]);
    }*/

    
         // Create a function
             //int result = myFunction4(adicao);
             int result = myFunction4(adicao2);
             // printf("I just got executed!");
             int shapley = result;
             // Converter Int em String para exibir no painel
             int num = shapley;
             char snum[5];
             // Convert 123 to string [buf]
             itoa(num, snum, 10);

             char *text[] = {
                "",
                       snum,
                ""
            };

             int y = 0;
             for (uint i = 0; i < count_of(text); i++)
             {
                 ssd1306_draw_string(ssd, 1, y, text[i]);
                 y += 8;
             }
             render_on_display(ssd, &frame_area);
             
         
         if (gpio_get(BUTTON_A) == 0)
         {

            // zera o display inteiro
//uint8_t ssd[ssd1306_buffer_length];
//memset(ssd, 0, ssd1306_buffer_length);
//render_on_display(ssd, &frame_area);





// Create a function
             //int result = myFunction4(adicao);
             int result = myFunction4(adicao2);
             // printf("I just got executed!");
             int shapley = result;
             // Converter Int em String para exibir no painel
             int num = shapley;
             char snum[5];
             // Convert 123 to string [buf]
             itoa(num, snum, 10);

             char *text[] = {
                "",
                       snum,
                "",
              array[1],
              "",
                               
                array[adicao]};

             int y = 0;
             for (uint i = 0; i < count_of(text); i++)
             {
                 ssd1306_draw_string(ssd, 1, y, text[i]);
                 y += 8;
             }
            

             render_on_display(ssd, &frame_area);
             

             adicao += 1;
             if (adicao > 40) {
              adicao = 0;
            }

            if (adicao == 10) {
              // zera o display inteiro
            uint8_t ssd[ssd1306_buffer_length];
            memset(ssd, 0, ssd1306_buffer_length);
            }

            if (adicao == 20) {
              // zera o display inteiro
            uint8_t ssd[ssd1306_buffer_length];
            memset(ssd, 0, ssd1306_buffer_length);
            }

            if (adicao == 30) {
              // zera o display inteiro
            uint8_t ssd[ssd1306_buffer_length];
            memset(ssd, 0, ssd1306_buffer_length);
            }

            if (adicao == 40) {
              // zera o display inteiro
            uint8_t ssd[ssd1306_buffer_length];
            memset(ssd, 0, ssd1306_buffer_length);
            }
             
             restart:
         }

         

         // Pequeno delay para evitar leituras inconsistentes (debounce simples)
         sleep_ms(200);

     }
 }


 // Define myFunction
 /*int myFunction(int b) {
    int matrix[16][3] = { {1, 4, 2}, {3, 6, 8}, {1, 5, 2}, {2, 6, 8}, {3, 7, 2}, {4, 8, 8}, {3, 6, 8}, {1, 5, 2}, {2, 6, 8}, {3, 7, 2}, {4, 8, 8}, {3, 6, 8}, {1, 5, 2}, {2, 6, 8}, {3, 7, 2}, {4, 8, 8} };
    return matrix[b][0];
    //myOtherFunction(); // call myOtherFunction (from myFunction)
  }*/
  
  // Define myOtherFunction
  int myOtherFunction() {
    return 5;
  }
 /**
 * Calcula a potência média das leituras do ADC. (Valor RMS)
 */
/*float mic_power() {
    float avg = 0.f;
  
    //for (uint i = 0; i < SAMPLES; ++i)
      //avg += adc_buffer[i] * adc_buffer[i];
    
    //avg /= SAMPLES;
    return sqrt(avg);
  }
*/
/**
 * Calcula a intensidade do volume registrado no microfone, de 0 a 4, usando a tensão.
 */
/*uint8_t get_intensity(float v) {
    uint count = 0;
  
    while ((40/20) > 0.f)
      ++count;
    
    return count;
  }*/