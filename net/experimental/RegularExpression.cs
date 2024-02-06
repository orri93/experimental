using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Text;

namespace experimental
{
    internal class RegularExpression
    {
        private const string Pattern = @"([a-zA-Z]+)\s*(\d+)";

        public static void Experiment()
        {
            (string, string) tool;
            tool = Parse("EMS 3414");
        }

        private static (string, string) Parse(string input)
        {
            string type = null, guid = null;
            MatchCollection matchs;
            matchs = Regex.Matches(input, Pattern);
            return (type, guid);
        }
    }
}
