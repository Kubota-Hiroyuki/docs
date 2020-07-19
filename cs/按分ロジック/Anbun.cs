using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace ConsoleApp4
{
    public class AnbunCalc {
        public double Len { set; get; }
        public int Anbun { set; get; }
        public AnbunCalc(double len, int anbun) {
            Len = len;
            Anbun = anbun;
        }
    } 


    class Program
    {
        static void Main(string[] args)
        {
            var lenList = new List<double>() { 300 };
            var maxNode = 11;
            var stdLen = 30.0d;
            Anbun(lenList, maxNode, stdLen);
        }

        static void Anbun(List<double> lenList, int maxNode, double stdLen)
        {

            var dtList = new List<AnbunCalc>();
            foreach (var len in lenList)
            {
                dtList.Add(new AnbunCalc(len, 1));
            }

            while (true)
            {
                if (dtList.Select(x => x.Anbun).Sum() == maxNode)
                    break;
                var lst = dtList.OrderByDescending(x => x.Len / x.Anbun).FirstOrDefault();
                if (lst.Len/lst.Anbun <= stdLen)
                    break;
                lst.Anbun++;
            }

            foreach (var dt in dtList)
            {
                var log = $"[{dt.Len.ToString("F1")}][{dt.Anbun.ToString()}][{(dt.Len / dt.Anbun).ToString("F1")}]";
                Console.WriteLine(log);
                Debug.WriteLine(log);
            }
            return;

        }
    }
}

