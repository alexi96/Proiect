
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Application {

    private static final String ADD_PATH = "Adunare.exe";
    private static final String MUL_PATH = "Inmultire.exe";

    public static String exec(String command) throws IOException {
        Runtime rt = Runtime.getRuntime();
        Process proc = rt.exec(command);
        try (BufferedReader stdInput = new BufferedReader(new InputStreamReader(proc.getInputStream()))) {
            String res = "";
            String t = null;
            while ((t = stdInput.readLine()) != null) {
                res += t;
            }
            return t;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static String add(String x, String y) throws IOException {
        return Application.exec("mpiexec -n 2 " + ADD_PATH + " " + x + " " + y);
    }

    public static String mul(String x, String y) throws IOException {
        return Application.exec("mpiexec -n 2 " + MUL_PATH + " " + x + " " + y);
    }

    public static void main(String[] args) throws IOException {
        String start = "1234";
        String ration = "10";
        String x = start;
        int times = 100;
            
        System.out.println(x);
        for (int i = 0; i < times; i++) {
            x = mul(x, ration);
            System.out.println(x);
        }
    }
}
