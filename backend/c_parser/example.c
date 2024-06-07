int* func(char* lol) {
  int* x = malloc(sizeof(int));
  return x;
}

int main() {
  int* x = func("lol");
  while (*x > 13) {
    x += 3;
  }

  return 0;

}
