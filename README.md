# EmbarcaTech
Este projeto visa a implementação de um sistema de entregas via drones, utilizando do Direct Memory Access (DMA) para a tomada de decisão em tempo real, com a finalidade de otimizar os ganhos e viabilizar o modelo de negócio proposto.
Trata-se de um sistema que coordena múltiplos agentes (drones, aeroportos, clientes e lojas) buscando uma harmonia entre o tempo e os recursos necessários para cada entrega, de modo que a soma das partes (coalizão) garanta maior ganho para os agentes do que eles agindo individualmente (jogo superaditivo).
Para conseguir tal resultado é necessário um cálculo dinâmico com aproximadamente 50 variáveis, que seja rápido suficiente para enviar um lance no leilão do sistema em tempo hábil. O leilão é fundamentado num mecanismo VCG (Vickrey-Clarke-Groves), usado para alocar bens ou serviços entre agentes de forma eficiente.
O módulo princiapal de decisão foi implementado eonstruído considerando exclusivamente os recursos presentes na placa BitDogLab (manipulador DMA, Botões e Display OLED), sendo ela essencial para a velocidade de processamento do cálculo e, posteriormente, a entrada e a saída dos dados em nuvem.
