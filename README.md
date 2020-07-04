# Boaster

Boaster is a software rasterizer written in C, for the heck of it

## Build

Build with `make`:

```sh
make
```

### Dependencies

Boaster requires POSIX API's to be present. This means that _Windows is not supported_. However, _WSL is supported_.

> Feel free to issue a PR to port Boaster to Windows or other platforms!

## Usage

### Compilation

Boaster is made up of multiple modules. Each module compiles to a static library, which you can link against:

`-lboaster -lboastmath -lboastgl`

### API

Before rendering with Boaster, you need to create a _context_ by specifying a _configuration_:

```c
boaster_config_t config = {
    .raster_mode = THREADED,
    .num_threads = -1
};

boaster_context_t *context = boaster_context_create(config);
```

Once this is done, you can use this context to render. For rendering, a _draw call_ must be specified, which can then be submitted to the renderer:

```c
boaster_draw_call_t draw_call = {
    .vertex_shader = pass_through_vertex_shader,
    .input_format = &format,
    .transform_format = &format,
    .pixel_shader = pixel_shader,
    .vertex_buffer = vertex_buffer,
    .target_image = image
};

boaster_render(context, draw_call);
```

## Samples

Boaster comes with multiple sample applications, which can be found in the root or in the parrotdemo folder:

* `./out/main`
* `./out/glmain`
* `./out/executor`
* `./parrotdemo/out/parrot`
  * Must be run from `./parrotdemo`
  * This sample contains multiple animations
  * The animation can be specified as a command line parameter
  * Available animations ( under `./parrotdemo/data/anim` ): `parrot`, `shiftcube`

> NOTE: From WSL, specify the display to use: `DISPLAY=:0 ./out/glmain`

## License

Unless otherwise specified, the MIT license applies to all code and assets in this repository. See [LICENSE](LICENSE).

Credit is appreciated but optional. :) 

## Contribution

Feel free to submit a [pull request](https://github.com/elementbound/boaster/pulls) for review!
