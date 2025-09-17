
.. toctree::
   :maxdepth: 3
   :caption: KS0597 智慧校园学习套件

   README.md
   docs/1.Product Introduction.md
   docs/2.Product Installation.md
   docs/3.Driver Installation/3. Driver Installation (Optional).md
   docs/4.Arduino Projects/Arduino.rst
   docs/5.Kidsblock Projects/KidsBlock.rst
   docs/6.Resources Download.md
   docs/7.Troubleshooting.md


.. mermaid::

    A([START]) --> B[Initialization]
    B --> C[Read the photoresistor value]
    C --> D[Serial monitor displays data]
    D --> E{light intensity}
    E -- YES --> F[Turn on LED]
    E -- NO --> G[Turn off LED]
    F \& G --> H[Delay 500ms]
    H --> C[Read the photoresistor value]
















