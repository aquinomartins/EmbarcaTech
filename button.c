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

#define LED_BLUE 12  // GPIO conectado ao terminal azul do LED RGB
#define LED_GREEN 11 // GPIO conectado ao terminal verde do LED RGB
#define BUTTON_A 5   // GPIO conectado ao Botão A
#define BUTTON_B 6   // GPIO conectado ao Botão A

#define ROWS 1000 // Número máximo de palavras
#define COLS 20   // Tamanho máximo de cada palavra

int myFunction4(int a)
{
  return a;
}

/*float mic_power();
 */

int adicao = 0;
int adicao2 = 0;

int main()
{

  // Configuração do GPIO do LED como saída
  gpio_init(LED_BLUE);
  gpio_set_dir(LED_BLUE, GPIO_OUT);
  gpio_put(LED_BLUE, false); // Inicialmente, o LED está apagado

  // Configuração do GPIO do LED como saída
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
  gpio_put(LED_GREEN, false); // Inicialmente, o LED está apagado

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

  // Get a free channel, panic() if there are none
  int chan = dma_claim_unused_channel(true);

  // Transferências de 8 bits. Tanto o endereço de leitura quanto o de escrita são incrementados após cada transferência (cada um apontando para um local em src ou dst, respectivamente).
  // Nenhum DREQ foi selecionado, então o DMA transfere o mais rápido possível.

  dma_channel_config c = dma_channel_get_default_config(chan);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
  channel_config_set_read_increment(&c, true);
  channel_config_set_write_increment(&c, true);

  char *text[] = {
      "",
      "      Entregas",
      "",
      "",
      "",
      "",
      "",
      "Aperte Botao A"};

  int y = 0;
  for (uint i = 0; i < count_of(text); i++)
  {
    ssd1306_draw_string(ssd, 1, y, text[i]);
    y += 8;
  }

  ssd1306_draw_line(ssd, 0, 0, 120, 0, true);
  ssd1306_draw_line(ssd, 0, 53, 120, 53, true);
  ssd1306_draw_line(ssd, 0, 18, 40, 18, true);
  ssd1306_draw_line(ssd, 40, 18, 40, 0, true);
  render_on_display(ssd, &frame_area);

  while (true)
  {

    adicao2 += 1;

    dma_channel_configure(
        chan,  // Canal a ser configurado
        &c,    // A configuração que acabamos de criar
        dst,   // O endereço inicial de escrita
        src,   // O endereço inicial de leitura
        50000, // Número de transferências; neste caso, cada uma é de 1 byte.
        true   // Iniciar imediatamente.
    );

    // Poderíamos optar por fazer outra coisa enquanto o DMA está executando sua tarefa.
    // Neste caso, o processador não tem mais nada para fazer, então apenas aguardamos o DMA concluir.

    dma_channel_wait_for_finish_blocking(chan);
    dma_channel_cleanup(chan);
    dma_channel_abort(chan);
    dma_channel_abort(chan);
    // O DMA agora copiou nosso texto do buffer de transmissão (src) para o
    // buffer de recepção (dst), então podemos imprimi-lo a partir de lá.

    char array[ROWS][COLS]; // Array 2D de palavras
    char *token;
    int i = 0;

    // Dividir a string em palavras
    token = strtok(dst, " ");
    while (token != NULL && i < ROWS)
    {
      strncpy(array[i], token, COLS - 1); // Copiar palavra para o array
      array[i][COLS - 1] = '\0';          // Garantir terminação nula
      i++;
      token = strtok(NULL, " ");
    }

    // Lê o estado do Botão A
    bool button_a_state = gpio_get(BUTTON_A); // HIGH = solto, LOW = pressionado
    bool button_b_state = gpio_get(BUTTON_B);
    // Atualiza o estado do LED com base no estado do Botão A
    gpio_put(LED_BLUE, button_a_state); // Se solto (HIGH), LED acende; se pressionado (LOW), apaga
    gpio_put(LED_GREEN, false);

    int result = myFunction4(adicao2 / 5);
    // printf("I just got executed!");
    int shapley = result;
    // Converter Int em String para exibir no painel
    int num = shapley;
    char snum[5];
    // Convert 123 to string [buf]
    itoa(num, snum, 10);

    char *text[] = {
        "",
        snum

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

      // Create a function

      itoa(num, snum, 10);

      char *text[] = {
          "",
          "",
          "",
          array[adicao + 1],
          array[adicao + 2],
          array[adicao + 3],

          "",
          "escolha Botao B"};

      int y = 0;
      for (uint i = 0; i < count_of(text); i++)
      {
        ssd1306_draw_string(ssd, 1, y, text[i]);
        y += 8;
      }

      render_on_display(ssd, &frame_area);

      adicao += 41;
      if (adicao > 410)
      {
        adicao = 0;
      }

      if (adicao == 10)
      {
        // zera o display inteiro
        uint8_t ssd[ssd1306_buffer_length];
        memset(ssd, 0, ssd1306_buffer_length);
      }

      if (adicao == 20)
      {
        // zera o display inteiro
        uint8_t ssd[ssd1306_buffer_length];
        memset(ssd, 0, ssd1306_buffer_length);
      }

      if (adicao == 30)
      {
        // zera o display inteiro
        uint8_t ssd[ssd1306_buffer_length];
        memset(ssd, 0, ssd1306_buffer_length);
      }

      if (adicao == 40)
      {
        // zera o display inteiro
        uint8_t ssd[ssd1306_buffer_length];
        memset(ssd, 0, ssd1306_buffer_length);
      }

    restart:
    }

    if (gpio_get(BUTTON_B) == 0)
    {

      // zera o display inteiro
      uint8_t ssd[ssd1306_buffer_length];
      memset(ssd, 0, ssd1306_buffer_length);
      render_on_display(ssd, &frame_area);

      gpio_put(LED_BLUE, false);
      gpio_put(LED_GREEN, button_a_state);

      char *text[] = {
          array[adicao + 1 - 41],
          "para",
          array[adicao + 2 - 41],
          array[adicao + 4 - 41],
          "",
          array[adicao + 21 - 41],
          array[adicao + 3 - 41],

          "",
          "Voltar Cristo A"};

      int y = 0;
      for (uint i = 0; i < count_of(text); i++)
      {
        ssd1306_draw_string(ssd, 1, y, text[i]);
        y += 8;
      }

      render_on_display(ssd, &frame_area);

      sleep_ms(2000);
    }

    // Pequeno delay para evitar leituras inconsistentes (debounce simples)
    sleep_ms(200);
  }
}

// Define myOtherFunction
int myOtherFunction()
{
  return 5;
}
