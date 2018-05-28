package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class BenchmarkUI extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws IOException {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("Chart.fxml"));
        Parent root = loader.load();

        primaryStage.setTitle("Microbenchmarks");
        primaryStage.setScene(new Scene(root, 800, 600));
        primaryStage.show();
    }
}
