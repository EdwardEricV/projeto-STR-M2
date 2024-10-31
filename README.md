
# Trabalho de STR 

## Problematica
Uma fabricante de automóveis quer testar um novo sistema de monitoramento do comportamento
de várias áreas de um veículo no qual ela está projetando. O projeto é novo e permitirá controle e
noção do que está acontecendo em todo o veículo. A empresa, em um primeiro momento, dividiu o
carro em 3 subsistemas: (i) motor; (ii) frenagem; e (iii) equipamentos de suporte a vida; No motor,
deve-se monitorar e controlar a injeção eletrônica e temperatura interna (2 sensores e 2 atuadores).
Na frenagem, deve-se monitorar e controlar o ABS nas duas rodas dianteiras (1 sensor e 1 atuador).
Nos equipamentos de suporte a vida, deve-se monitorar e controlar o airbag e cinto de segurança
(2 sensores e 2 atuadores). Todos os sistemas se conectam a um computador de bordo central,
que deve fornecer, reter e controlar ações destes sistemas. Cada sistema conta com um sensor e
um controlador (totalizando 5 sensores e 5 controladores). Você deve assumir que todos os
sensores e controladores estão na mesma distância (1 m) do computador de bordo e que o tempo
de envio de dados é de 1 Mbps, sendo que os dados podem demorar até 10us para se propagar
pelo fio (do nodo até o computador). Os deadlines relativos para uma ação de controle (após
detecção do sensor) são:

* Injeção eletrônica: 500 us após alteração no pedal
* Temperatura do motor: 20 ms após detecção de temperatura acima do limite
* ABS: 100 ms após acionamento no pedal
* Airbag: 100 ms após detecção de choque
* Cinto de segurança: 1 segundo após carro em movimento

O computador de bordo deve também alimentar um display para o usuário com alertas de
informações (mensagens com caracteres). A atualização do display deverá sempre ser de 1s.
A empresa gostaria de monitorar todos os sensores indicados acima e, após amostrar todos os
sensores, exibir em um display (ou tela) o estado dos sensores (que indicam o estado dos
subsistemas monitorados). A empresa exige que desenvolva sua solução para trabalhar utilizando
o FreeRTOS na placa ESP32 e SDK ESP-IDF. Considere também que o sensor demora 1 us para
fazer a aquisição da amostra corretamente e que o controlador demora 5 us para agir. Além disso,
agora ela gostaria de monitorar a velocidade média e o consumo médio de gasolina e atualizar a
cada instante no sensor. Para isso, ela diz que uma boa amostragem do motor é quando são
colhidas (para os dois casos) 200 amostras. A atualização que ela acha adequada no display,
segundo pesquisas com usuários, é de 1 segundo.
