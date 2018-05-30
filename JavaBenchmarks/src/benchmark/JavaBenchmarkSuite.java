package benchmark;

import com.sun.org.apache.xpath.internal.SourceTree;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class JavaBenchmarkSuite {
    private static int ARRAY_SIZE = 1000;
    private static int THREAD_SLEEP = 50;

    private double measureStaticMemoryAllocation(int n){
        long sum = 0;
        for(int i = 0; i< n ; i++){
            sum += staticMem();
        }
        return sum / n;
    }

    private double staticMem(){
        int[] randomArray = new int[ARRAY_SIZE];
        Random rand = new Random();

        long dummyStart = System.nanoTime();
        System.nanoTime();
        long dummyDuration = System.nanoTime() - dummyStart;

        long start = System.nanoTime();
        int[] local = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        long stop = System.nanoTime();

        randomArray[rand.nextInt(ARRAY_SIZE)] = local.length;
        return stop - start- dummyDuration;
    }

    private double measureDynamicMemoryAllocation(int n){
        int[][] matrix = new int[ARRAY_SIZE][];

        long start = System.nanoTime();
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[ARRAY_SIZE];
        }

        long stop = System.nanoTime();
        long elapsed = stop - start;

        //do something with matrix so that the compiler does not optimize it away
        Random rand = new Random();
        int index = rand.nextInt(ARRAY_SIZE);
        int index1 = rand.nextInt(ARRAY_SIZE);
        matrix[index][index1] *= rand.nextInt(n);

        return elapsed / n;
    }

    private double measureMemoryAccess(int n){
        int[] arr = new int[ARRAY_SIZE];

        long start = System.nanoTime();
        for(int j = 0; j< n; j++) {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                arr[i]++;
            }
        }
        long stop = System.nanoTime();
        long elapsed = stop - start;

        Random rand = new Random();
        arr[rand.nextInt(n)] *= rand.nextInt(n);

        return elapsed / n;
    }

    public double measureThreadCreation(int n){
        double duration = 0.0;

        try {
            for(int i = 0; i< n; i++)
                duration += createThread();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return duration / n;
    }

    private void foo(){
        int[] arr = new int[ARRAY_SIZE];
        for (int j = 0; j < ARRAY_SIZE; j++) {
            arr[j] = 1;
        }
    }

    public double createThread() throws InterruptedException {
        long fooStart = System.nanoTime();
        foo();
        long fooDuration = System.nanoTime() - fooStart;

        long start = System.nanoTime();
        Thread t = new Thread(() -> {foo();});
        t.start();
        t.join();
        long end = System.nanoTime();
        return end - start - fooDuration;
    }

    void sleep_foo(){
        try{
            Thread.sleep(THREAD_SLEEP);
        } catch (InterruptedException e){

        }
    }

    double measureContextSwitch(int n){
        List<Thread> threads = new ArrayList<>();

        long start = System.nanoTime();
        for(int i = 0; i < n; i++){
            threads.add(new Thread(() -> {sleep_foo();}));
        }

        for(Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        long end = System.nanoTime();
        return (end - start - (THREAD_SLEEP * 1000000))/ n;

    }

    long context_end = 0;

    void context_foo(){
        context_end = System.nanoTime();
    }

    double contextSwitch() throws InterruptedException {
        long start = System.nanoTime();
        Thread t = new Thread(() -> {context_foo();});
        t.setPriority(Thread.MAX_PRIORITY);
        t.start();
        t.join();

        return context_end - start;
    }

    double measureContextSwitch2(int n){
        double total = 0.0;
        try{
            for(int i = 0; i < n; i++)
                total += contextSwitch();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return total / n;
    }

    public void RunBenchmarks(int n) {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter("C:\\Users\\zenbookx\\Documents\\Facultate\\An III\\Sem II\\SCS\\project\\JavaBenchmarks\\ java_benchmarks_results.csv"));
            bw.write(n + "\n");
            bw.write("STATICMEM, Java, " + String.format("%.3f", measureStaticMemoryAllocation(n) /1000.0 ) + " us\n");
            bw.write("DYNAMICMEM, Java, " + String.format("%.3f", measureDynamicMemoryAllocation(n) /1000.0) + " us\n");
            bw.write("MEMACCESS, Java, " + String.format("%.3f",measureMemoryAccess(n) / 1000.0) + " us\n");
            bw.write("THREADCREAT, Java, " + String.format("%.3f",measureThreadCreation(n) / 1000.0) + " us\n");
            bw.write("CONTEXTSW, Java, " + String.format("%.3f",measureContextSwitch2(n) / 1000.0) + " us\n");
            bw.close();
            System.out.println("Java results written to csv.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String args[]){
        JavaBenchmarkSuite benchmarkSuite = new JavaBenchmarkSuite();
        int n;
        if(args.length > 0)
            n = Integer.parseInt(args[0]);
        else
            n = 1000;
        benchmarkSuite.RunBenchmarks(n);
    }
}
