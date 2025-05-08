ImGUI virtual keyboard: https://github.com/le0nklcpp/imgui-mobile-keyboard

**Few words about ImGUI and how I came to this project**

ImGUI is a Bloat-free UI framework which lacks elegance _by default_, but allows you to effectively build an application UI. I tried that and I liked the idea.

You guys heard of Emscripten? It's a great compiler that allows running C/C++ code in your browser.
Even more, there's a lot of cool stuff ported to Emscripten,
including graphics libraries and UI frameworks. So, you can actually create a web application using Emscripten and it will utilize WASM and use WebGL
to draw your few buttons, which sounded crazy enough to me, so I decided to give it a try. I almost liked the result.

**Problem**

ImGUI web version doesn't support an on-screen keyboard for mobile devices, so the worst way to fix that was by creating a virtual keyboard for ImGUI. 
And that's what I did.

**How is that different from what we have now for ImGUI**

There are a few other open-source OSK solutions for ImGUI, but they are non-customizable, complex or don't look ~~like ass~~ modern.
So I decided to create my own solution which is in this repo above. 
It is not 100% ready but looks mobile enough, can be used to type text in your language if you add it,that's relatively easy.
As this project is not my \#1 priority you can consider it abandoned
