extern void app(void);

__attribute__((section("vectors"))) void ResetHandler(void) {
    app();
}
