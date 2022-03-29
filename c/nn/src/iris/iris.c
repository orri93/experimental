#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <genann.h>

#define LINE_BUFFER_SIZE 256

enum iris_class {
  Undefined,
  Setosa,
  Versicolour,
  Virginica
};

struct iris {
  double sepal_length;
  double sepal_width;
  double petal_length;
  double petal_width;
  enum iris_class class;
};

const char *class_names[] = {
  "Iris-setosa",
  "Iris-versicolor",
  "Iris-virginica"
};

static int load_data(const char* file_name, struct iris** iris_array);
static void split_data(
  struct iris* iris_array,
  struct iris* training_array,
  struct iris* verifying_array,
  int count,
  int* training,
  int* verifying);
static void train_from_data(
  genann* ann,
  struct iris* iris_array,
  int count,
  int loop,
  double learning_rate);
static void verify_from_data(genann* ann, struct iris* iris_array, int count);
static enum iris_class class_from_guess(const double* guess);
static int maximum_index(const double* number, int count);

int main(int argc, char *argv[]) {
  int count, training, verifying;
  int result = EXIT_SUCCESS;
  struct iris* iris_array = 0;
  struct iris* training_array = 0;
  struct iris* verifying_array = 0;
  genann* ann;

  /* Load the data from file. */
  count = load_data(argc > 1 ? argv[1] : 0, &iris_array);

  if (count > 0) {
    training_array = malloc(sizeof(struct iris) * count);
    verifying_array = malloc(sizeof(struct iris) * count);
    split_data(
      iris_array,
      training_array,
      verifying_array,
      count,
      &training,
      &verifying);

    /* 4 inputs.
     * 2 hidden layer(s) of 4 neurons.
     * 3 outputs (1 per class)
     */
    ann = genann_init(4, 2, 4, 3);
    if (ann) {
      // train_from_data(ann, training_array, training, 1000, 0.01); // 298 ms no failure
      train_from_data(ann, training_array, training, 500, 0.02);     // 104 ms no failure
      // train_from_data(ann, training_array, training, 250, 0.04);  // 41 ms one failure

      verify_from_data(ann, verifying_array, verifying);
    
      genann_free(ann);
    }
    else {
      fprintf(stderr, "Failed to initialize the Neural Network\n");
      result = EXIT_FAILURE;
    }
  } else {
    result = EXIT_FAILURE;
  }

  if (iris_array) {
    free(iris_array);
  }
  if (training_array) {
    free(training_array);
  }
  if (verifying_array) {
    free(verifying_array);
  }

  return result;
}

int load_data(const char* file_name, struct iris** iris_array) {
  int i, j;
  int samples;
  char* split;
  char line[LINE_BUFFER_SIZE];
  double number[4];
  struct iris* iris_at;
  FILE* in;

  if (file_name) {
    in = fopen(file_name, "r");
    if (!in) {
      fprintf(stderr, "Could not open file: %s\n", file_name);
      return 0;
    }
  }
  else {
    in = fopen("iris.data", "r");
    if (!in) {
      fprintf(stderr, "Could not open file: iris.data\n");
      return 0;
    }
  }

  /* Loop through the data to get a count. */
  samples = 0;
  while (!feof(in) && fgets(line, LINE_BUFFER_SIZE, in)) {
    ++samples;
  }
  fseek(in, 0, SEEK_SET);

  printf("Loading %d data points\n", samples);

  *iris_array = malloc(sizeof(struct iris) * samples);
  iris_at = *iris_array;

  /* Read the file into the iris array */
  for (i = 0; i < samples; ++i) {
    if (fgets(line, LINE_BUFFER_SIZE, in) == NULL) {
      perror("fgets");
      return 0;
    }

    split = strtok(line, ",");
    for (j = 0; j < 4; ++j) {
      number[j] = atof(split);
      split = strtok(0, ",");
    }
    iris_at->sepal_length = number[0];
    iris_at->sepal_width = number[1];
    iris_at->petal_length = number[2];
    iris_at->petal_width = number[3];
    iris_at->class = Undefined;

    split[strlen(split) - 1] = 0;
    if (strcmp(split, class_names[0]) == 0) {
      iris_at->class = Setosa;
    }
    else if (strcmp(split, class_names[1]) == 0) {
      iris_at->class = Versicolour;
    }
    else if (strcmp(split, class_names[2]) == 0) {
      iris_at->class = Virginica;
    }
    else {
      fprintf(stderr, "Unknown class %s.\n", split);
      return 0;
    }
    iris_at++;
  }

  return samples;
}

void split_data(
  struct iris* iris_array,
  struct iris* training_array,
  struct iris* verifying_array,
  int count,
  int* training,
  int* verifying) {

  int i;
  int training_setosa, training_versicolor, training_virginica;
  int count_setosa = 0, count_versicolor = 0, count_virginica = 0;

  for (i = 0; i < count; ++i) {
    switch (iris_array[i].class) {
    case Setosa:
      count_setosa++;
      break;
    case Versicolour:
      count_versicolor++;
      break;
    case Virginica:
      count_virginica++;
      break;
    }
  }

  training_setosa = 4 * count_setosa / 5;
  training_versicolor = 4 * count_versicolor / 5;
  training_virginica = 4 * count_virginica / 5;

  count_setosa = 0;
  count_versicolor = 0;
  count_virginica = 0;

  *training = 0;
  *verifying = 0;

  for (i = 0; i < count; ++i) {
    switch (iris_array->class) {
    case Setosa:
      if (count_setosa < training_setosa) {
        count_setosa++;
        memcpy(training_array, iris_array, sizeof(struct iris));
        training_array++;
        (*training)++;
      } else {
        memcpy(verifying_array, iris_array, sizeof(struct iris));
        verifying_array++;
        (*verifying)++;
      }
      break;
    case Versicolour:
      if (count_versicolor < training_versicolor) {
        count_versicolor++;
        memcpy(training_array, iris_array, sizeof(struct iris));
        training_array++;
        (*training)++;
      }
      else {
        memcpy(verifying_array, iris_array, sizeof(struct iris));
        verifying_array++;
        (*verifying)++;
      }
      break;
    case Virginica:
      if (count_virginica < training_virginica) {
        count_virginica++;
        memcpy(training_array, iris_array, sizeof(struct iris));
        training_array++;
        (*training)++;
      }
      else {
        memcpy(verifying_array, iris_array, sizeof(struct iris));
        verifying_array++;
        (*verifying)++;
      }
      break;
    }
    iris_array++;
  }
}

void train_from_data(genann* ann, struct iris* iris_array, int count, int loop, double learning_rate) {
  int n, i;
  double inputs[4];
  double outputs[3];
  clock_t start, elapsed;

  start = clock();
  for (n = 0; n < loop; ++n) {
    for (i = 0; i < count; ++i) {
      inputs[0] = iris_array[i].sepal_length;
      inputs[1] = iris_array[i].sepal_width;
      inputs[2] = iris_array[i].petal_length;
      inputs[3] = iris_array[i].petal_width;
      switch (iris_array[i].class) {
      case Setosa:
        outputs[0] = 1.0;
        outputs[1] = 0.0;
        outputs[2] = 0.0;
        break;
      case Versicolour:
        outputs[0] = 0.0;
        outputs[1] = 1.0;
        outputs[2] = 0.0;
        break;
      case Virginica:
        outputs[0] = 0.0;
        outputs[1] = 0.0;
        outputs[2] = 1.0;
        break;
      }
      genann_train(ann, inputs, outputs, learning_rate);
    }
  }
  elapsed = clock() - start;
  printf("Training from data completed in %d ms\n", elapsed);
}

void verify_from_data(genann* ann, struct iris* iris_array, int count) {
  int i, j;
  double inputs[4];
  const double* guess;
  enum iris_class guess_class;

  for (i = 0; i < count; ++i) {
    inputs[0] = iris_array[i].sepal_length;
    inputs[1] = iris_array[i].sepal_width;
    inputs[2] = iris_array[i].petal_length;
    inputs[3] = iris_array[i].petal_width;
    for (j = 0; j < 4; ++j) {
      printf("%f,", inputs[j]);
    }
    guess = genann_run(ann, inputs);
    guess_class = class_from_guess(guess);
    switch (iris_array[i].class) {
    case Setosa:
      printf("Setosa");
      break;
    case Versicolour:
      printf("Versicolour");
      break;
    case Virginica:
      printf("Virginica");
      break;
    }
    for (j = 0; j < 3; ++j) {
      printf(",%f", guess[j]);
    }
    switch (guess_class) {
    case Setosa:
      printf(",Setosa");
      break;
    case Versicolour:
      printf(",Versicolour");
      break;
    case Virginica:
      printf(",Virginica");
      break;
    }
    if (iris_array[i].class == guess_class) {
      printf(",1");
    } else {
      printf(",0");
    }
    printf("\n");
  }
}

enum iris_class class_from_guess(const double* guess) {
  int index = maximum_index(guess, 3);
  switch (index) {
  case 0:
    return Setosa;
  case 1:
    return Versicolour;
  case 2:
    return Virginica;
  default:
    return Undefined;
  }
}

int maximum_index(const double* number, int count) {
  int i;
  int index = -1;
  double value = -FLT_MAX;
  for (i = 0; i < count; ++i) {
    if (number[i] > value) {
      index = i;
      value = number[i];
    }
  }
  return index;
}
