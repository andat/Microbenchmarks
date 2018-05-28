package sample;

import javafx.beans.InvalidationListener;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.geometry.Bounds;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.TextArea;
import javafx.scene.text.Text;

import java.io.*;
import java.net.URL;
import java.util.*;

public class ChartController implements Initializable{
    @FXML
    BarChart barChart;

    @FXML
    CategoryAxis xAxis;

    @FXML
    NumberAxis yAxis;

    @FXML
    TextArea javaLog;

    @FXML
    TextArea cppLog;

    @FXML
    TextArea cLog;

    private final String jarPath = "\"C:/Users/zenbookx/Documents/Facultate/An III/Sem II/SCS/project/JavaBenchmarks/out/artifacts/JavaBenchmarks_jar/JavaBenchmarks.jar\"";
    private final String cExecPath = "C:\\Users\\zenbookx\\Documents\\Facultate\\An III\\Sem II\\SCS\\project\\BenchmarkApplication\\CBenchmarks.exe";
    private final String cppExecPath= "C:\\Users\\zenbookx\\Documents\\Facultate\\An III\\Sem II\\SCS\\project\\BenchmarkApplication\\CppBenchmarks.exe";

    private XYChart.Series javaSeries;
    private XYChart.Series cppSeries;
    private XYChart.Series cSeries;

    private static String staticMem = "Static mem. alloc.";
    private static String dynamicMem = "Dynamic mem. alloc.";
    private static String memAccess = "Memory access";
    private static String threadCreat = "Thread creation";
    private static String contextSw = "Context switch";

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        xAxis.setLabel("Programming Language");

        //yAxis.setAutoRanging(true);
        yAxis.setLowerBound(0.0);
        yAxis.setUpperBound(1.0);
        yAxis.setTickUnit(0.00001);
        yAxis.setLabel("Execution time(ms)");

        //initialize series
        this.javaSeries = new XYChart.Series();
        javaSeries.setName("Java");

        this.cppSeries = new XYChart.Series();
        cppSeries.setName("C++");

        this.cSeries = new XYChart.Series();
        cSeries.setName("C");

        this.barChart.setBarGap(3);
        this.barChart.setCategoryGap(30);
        this.barChart.setAnimated(false);
    }

    public void runBtnClicked(){
        barChart.getData().clear();
        try {
            //java
            ProcessBuilder pb = new ProcessBuilder("java", "-jar", jarPath);
            Process p = pb.start();
            p.waitFor();

            //c
            ProcessBuilder pb1 = new ProcessBuilder(cExecPath);
            Process p1 = pb1.start();
            p1.waitFor();

            //cpp
            ProcessBuilder pb2 = new ProcessBuilder(cppExecPath);
            Process p2 = pb2.start();
            p2.waitFor();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        fillJavaSeries();
        fillCppSeries();
        fillCSeries();

        barChart.getData().setAll(javaSeries, cppSeries, cSeries);
    }

    private void fillJavaSeries(){
        //java
        List<String> javaLines = readFromFile("C:\\Users\\zenbookx\\Documents\\Facultate\\An III\\Sem II\\SCS\\project\\JavaBenchmarks\\ java_benchmarks_results.csv");
        List<Double> javaValues = extractValues(javaLines);

        this.javaLog.setText("");
        this.javaLog.appendText("Java:\n");
        for(int i = 1; i< javaLines.size(); i++)
            javaLog.appendText(javaLines.get(i) + "\n");


        javaSeries.getData().add(new XYChart.Data<>(staticMem, javaValues.get(0)));
        javaSeries.getData().add(new XYChart.Data<>(dynamicMem, javaValues.get(1)));
        javaSeries.getData().add(new XYChart.Data<>(memAccess, javaValues.get(2)));
        javaSeries.getData().add(new XYChart.Data<>(threadCreat, javaValues.get(3)));
    }

    private void fillCppSeries(){
        List<String> cppLines = readFromFile("C:/Users/zenbookx/Documents/Facultate/An III/Sem II/SCS/project/CppBenchmarks/CppBenchmarks/cpp_benchmarks_results.csv");
        List<Double> cppValues = extractValues(cppLines);

        this.cppLog.setText("");
        this.cppLog.appendText("C++:\n");
        for(int i = 1; i< cppLines.size(); i++)
            cppLog.appendText(cppLines.get(i) + "\n");

        cppSeries.getData().add(new XYChart.Data<>(staticMem, cppValues.get(0)));
        cppSeries.getData().add(new XYChart.Data<>(dynamicMem, cppValues.get(1)));
        cppSeries.getData().add(new XYChart.Data<>(memAccess, cppValues.get(2)));
        cppSeries.getData().add(new XYChart.Data<>(threadCreat, cppValues.get(3)));
        cppSeries.getData().add(new XYChart.Data<>(contextSw, cppValues.get(4)));
    }

    private void fillCSeries(){
        List<String> cLines = readFromFile("C:/Users/zenbookx/Documents/Facultate/An III/Sem II/SCS/project/CBenchmarks/cmake-build-debug/c_benchmarks_results.csv");
        List<Double> cValues = extractValues(cLines);

        this.cLog.setText("");
        this.cLog.appendText("C:\n");
        for(int i = 1; i< cLines.size(); i++)
            cLog.appendText(cLines.get(i) + "\n");

        cSeries.getData().add(new XYChart.Data<>(staticMem, cValues.get(0)));
        cSeries.getData().add(new XYChart.Data<>(dynamicMem, cValues.get(1)));
        cSeries.getData().add(new XYChart.Data<>(memAccess, cValues.get(2)));
        cSeries.getData().add(new XYChart.Data<>(threadCreat, cValues.get(3)));
        cSeries.getData().add(new XYChart.Data<>(contextSw, cValues.get(4)));
    }


    private List<Double> extractValues(List<String> lines){
        List<Double> values = new ArrayList<>();
        for(int i = 1; i < lines.size(); i++){
            String[] tokens = lines.get(i).split(", ");
            String val = tokens[2].replaceAll("ms", "");
            values.add(Double.parseDouble(val));
        }
        return values;
    }

    private List<String> readFromFile(String filename){
        String line;
        List<String> lines = new ArrayList<>();

        try {
            FileReader fileReader = new FileReader(filename);
            BufferedReader bufferedReader = new BufferedReader(fileReader);

            while((line = bufferedReader.readLine()) != null) {
                System.out.println(line);
                lines.add(line);
            }
            bufferedReader.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("File " + filename + "not found");
        }
        catch(IOException ex) {
            ex.printStackTrace();
        }

        return lines;
    }
}
