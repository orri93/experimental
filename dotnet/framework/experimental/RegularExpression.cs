using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace experimental
{
    internal class RegularExpression
    {
        private const string Pattern = @"([a-zA-Z]+)\s*(\d+)";

        public static void Experiment()
        {
            string pattern = "dht0\\d{2}_type";
            Match match;

            match = System.Text.RegularExpressions.Regex.Match("dht000_tyoe", pattern);
            match = System.Text.RegularExpressions.Regex.Match("dht000_type", pattern);

            string type, guid;
            Parse("EMS 3414", out type, out guid);
        }

        private static bool Parse(string input, out string type, out string guid)
        {
            type = null;
            guid = null;
            Match match;
            match = Regex.Match(input, Pattern);
            if (match.Success)
            {
                if (match.Groups.Count > 1)
                {
                    type = match.Groups[1].Value;
                }
                if (match.Groups.Count > 2)
                {
                    guid = match.Groups[2].Value;
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
