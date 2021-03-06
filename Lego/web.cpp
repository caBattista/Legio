#include "arduino.h"

const String _index = R"=====(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Legio</title>
</head>

<body>
    <input id="slider" type="range" min="-100" max="100" value="0" class="slider" id="myRange">
    <input id="display" type="text">
    <div class="middle"></div>
    <style>
        body {
            margin: 0;
            overflow: hidden;
            width: 100%;
            height: 100%;
        }

        #display {
            position: absolute;
            top: calc(50% + 100px);
            left: 50%;
            transform: translateY(-50%);
            transform: translateX(-50%);
            border: none;
            background: #d3d3d3;
            text-align: center;
            font-size: 2em;
        }

        .slider {
            -webkit-appearance: none;
            width: 90%;
            height: 25px;
            background: #d3d3d3;
            outline: none;
            -webkit-transition: .2s;
            transition: opacity .2s;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translateY(-50%);
            transform: translateX(-50%);
        }

        .slider:hover {
            opacity: 1;
        }

        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            background: #4CAF50;
            cursor: pointer;
        }

        .slider::-moz-range-thumb {
            width: 25px;
            height: 25px;
            background: #4CAF50;
            cursor: pointer;
        }

        .middle {
            background-color: rgb(151, 151, 151);
            width: 1px;
            height: 100px;
            position: absolute;
            left: calc(50% + 1px);
            top: calc(50% - 35px);
            z-index: -1;
        }
    </style>
    <script>
        connection = new WebSocket('ws://' + location.hostname + ':85/', ['arduino']);
        connection.onopen = function () {
            connection.send('Connect ' + new Date());
        };
        connection.onerror = function (error) {
            console.log('WebSocket Error ', error);
        };
        connection.onmessage = function (e) {
            console.log('Server: ', e.data);
        };

        let input = document.querySelector("#slider");
        let display = document.querySelector("#display");
        let timer;
        input.addEventListener("mousedown", (ev) => {
            timer = setInterval(() => {
                display.value = input.value;
                connection.send(display.value);
            }, 20);
        })
        input.addEventListener("mouseup", (ev) => {
            if (input.value > -10 && input.value < 10) {
                input.value = display.value = 0;
                connection.send(0);
            }
            clearInterval(timer);
        })
        input.addEventListener("change", (ev) => {
                connection.send(input.value);
        })
    </script>
</body>

</html>
)=====";
