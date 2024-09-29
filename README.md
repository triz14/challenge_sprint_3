
# Challenge - Edge Computing Sprint 3
Projeto que simula um carrinho segue linha



## Participantes

```http 
- Abner de Paiva Barbosa          | rm558468
- Beatriz Vieira de Novais        | rm554746
- Fernando Luiz Silva Antonio     | rm55201
- Mariana Neugebauer Dourado      | rm550494
- Thomas de Almeida Reichmann     | rm554812
```




## Informações do projeto
Informações sobre alguns componentes usados na criação do projeto:

- ESP32 - é um microcontrolador que vem com um grande número de pinos de entrada/saída (E/S) que podem ser usados para controlar diversos componentes eletrônicos, como sensores, displays, motores, entre outro;
- Sensor PIR - consegue detectar o movimento de objetos que estejam em uma área de até 7 metros;
- RTC1307 -  é um chip de relógio em tempo real que computa segundos, minutos, horas, dias da semana, dias do mês, meses e anos;
- LED - é um componente eletrônico que emite luz quando uma corrente elétrica passa por ele;
- Resistor - sua função é de limitar o fluxo de corrente elétrica em um circuito. Eles podem ser usados para várias coisas, uma delas em exemplo: é limitar a corrente que passa por um LED para evitar danos;
- Jumpers - são cabos utilizados para fazer conexões entre os diversos componentes do circuito.
## Passo a Passo
Para você conseguir fazer esse projeto, você vai precisar desses componentes:

- 1 LED
- 1 Resistor
- Jumpers
- RTC1307
- PIR
- ESP32

Após você conseguir todos os matériais, você pode agora seguir os passos à passos.

2. produzindo conexão:

Conexões negativas:

- conecte a perna negativa do LED ao resistor e a outra perna do resistor ao GND do ESP32;
- Conecte a perna negativa do PIR ao GND;
- Conecte a entrada negativa do RTC1307 ao GND.

Conexões positivas: 

- Conecte a outra perna do LED a porta 15;
- Conecte a perna positiva do PIR ao 3.3V;
- Conecte a perna D do PIR a porta 12;
- Conecte a porta 5V do RTC a porta 3.3V;
- Conecte a SDA a porta 21;
- E por fim conecte o SDl a porta 22.

## Dependências (bibliotecas)
- PubSubClient;
- ArduinoJson;
- ArduinoJson;
- Você também deve configurar a conexão do wifi de acordo com a rede que vai usar no caso utilizei a própria rede do simulaodr, depois você deve configurar os tópicos seja ele para publicar ou receber informações.

## Explicação de como funciona o  projeto
Esse projeto é responsável por enviar informações via mqtt, dessa forma permitindo a comunicação dos sensores com outros programas e até mesmo comandos enviados pelo usuário, também utilizamos o node-red para exibir um dashboard das informações enviadas são elas data e hora 
## Link do projeto (simulação)
https://wokwi.com/projects/410014884664015873
